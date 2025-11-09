#include "vectordb/utils/logger/logger.h"

#include <memory>
#include <mutex>
#include <unordered_map>
#include <stdexcept>

#include "vectordb/utils/logger/logger/base.h"
#include "vectordb/utils/logger/logger/sync_logger.h"
#include "vectordb/utils/logger/formatter/default_formatter.h"
#include "vectordb/utils/logger/sink/base.h"

namespace vectordb::utils
{

namespace
{
    std::mutex logger_mutex;
    std::unordered_map<std::string, std::unique_ptr<Logger>> loggers;
}

Logger::Logger(const std::string & name, LogType type)
    : level(LogLevel::INFO)
    , type(type)
    , formatted(false)
    , formatter(nullptr)
    , logger(nullptr)
    , name(name)
{
    switch (type) {
        case LogType::SYNC:
            logger = std::make_unique<SyncLogger>(type);
            break;
        case LogType::ASYNC:
        default:
            throw std::invalid_argument("Unsupported logger type");
    }
}

Logger & Logger::getLogger(const std::string & name, LogType type)
{
    std::lock_guard<std::mutex> lock(logger_mutex);

    auto iter = loggers.find(name);
    if (iter != loggers.end()) {
        if (iter->second->logger && iter->second->logger->getType() != type) {
            throw std::invalid_argument("Logger with different type already exists: " + name);
        }
        return *(iter->second);
    }

    auto logger_instance = std::unique_ptr<Logger>(new Logger(name, type));
    auto [it, _] = loggers.emplace(name, std::move(logger_instance));
    return *(it->second);
}

Logger & Logger::getSyncLogger(const std::string & name)
{
    return getLogger(name, LogType::SYNC);
}

void Logger::addSink(std::shared_ptr<LogSinkBase> sink)
{
    // 添加输出器
    if (formatted) {
        sink->setFormatter(formatter);
    }

    // 添加输出器
    logger->addSink(sink);
}

void Logger::setLevel(LogLevel level)
{
    this->level = level;
}

void Logger::setFormatter(const std::string & pattern)
{
    // 保存格式化器模式
    formatter = std::make_shared<DefaultFormatter>(pattern);
    // 更新所有输出器
    logger->setFormatter(formatter);
    // 标记已格式化
    formatted = true;
}

void Logger::setFormatter(std::shared_ptr<LogFormatterBase> formatter)
{
    // 保存格式化器
    this->formatter = formatter;
    // 更新所有输出器
    logger->setFormatter(formatter);
    // 标记已格式化
    formatted = true;
}

void Logger::log(LogLevel level, const std::string & message, const char * file, unsigned int line, const char * function)
{
    // 如果日志级别小于当前级别，则不记录
    if (level < this->level) {
        return;
    }

    logger->log(name, level, message, file, line, function);
}

void Logger::debug(const std::string & message, const char * file, unsigned int line, const char * function)
{
    log(LogLevel::DEBUG, message, file, line, function);
}

void Logger::info(const std::string & message, const char * file, unsigned int line, const char * function)
{
    log(LogLevel::INFO, message, file, line, function);
}

void Logger::warn(const std::string & message, const char * file, unsigned int line, const char * function)
{
    log(LogLevel::WARN, message, file, line, function);
}

void Logger::error(const std::string & message, const char * file, unsigned int line, const char * function)
{
    log(LogLevel::ERROR, message, file, line, function);
}

void Logger::fatal(const std::string & message, const char * file, unsigned int line, const char * function)
{
    log(LogLevel::FATAL, message, file, line, function);
}

void Logger::flush()
{
    logger->flush();
}

} // namespace vectordb::utils
