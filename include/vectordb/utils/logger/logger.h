#pragma once

#include <memory>
#include <string>


#include "vectordb/utils/logger/common/type.h"
#include "vectordb/utils/logger/common/level.h"

namespace vectordb::utils
{

class LoggerBase;
class LogFormatterBase;
class LogSinkBase;

/**
 * @brief 日志记录器
 */
class Logger final
{
private:
    Logger() = delete;

    explicit Logger(const std::string & name, LogType type);

    Logger(const Logger &) = delete;

    Logger & operator=(const Logger &) = delete;

    Logger(Logger &&) = delete;

    Logger & operator=(Logger &&) = delete;

public:
    /**
     * @brief 获取日志记录器
     * @param name 日志名称
     * @param type 日志类型
     * @return 日志记录器
     */
    static Logger & getLogger(const std::string & name, LogType type);

    /**
     * @brief 获取同步日志记录器
     * @param name 日志名称
     * @return 同步日志记录器
     */
    static Logger & getSyncLogger(const std::string & name);

    /**
     * @brief 添加日志输出器
     * @param sink 日志输出器
     */
    void addSink(std::shared_ptr<LogSinkBase> sink);

    /**
     * @brief 设置日志级别
     * @param level 日志级别
     */
    void setLevel(LogLevel level);

    /**
     * @brief 设置日志格式化器模式
     * @param pattern 日志格式化器模式
     */
    void setFormatter(const std::string & pattern);

    /**
     * @brief 设置日志格式化器
     * @param formatter 日志格式化器
     */
    void setFormatter(std::shared_ptr<LogFormatterBase> formatter);

    /**
     * @brief 记录日志
     * @param level 日志级别
     * @param message 日志消息
     * @param file 文件名
     * @param line 行号
     * @param function 函数名
     */
    void log(LogLevel level, const std::string & message, const char * file = "", unsigned int line = 0, const char * function = "");

    /**
     * @brief 记录调试日志
     * @param message 日志消息
     * @param file 文件名
     * @param line 行号
     * @param function 函数名
     */
    void debug(const std::string & message, const char * file = "", unsigned int line = 0, const char * function = "");

    /**
     * @brief 记录信息日志
     * @param message 日志消息
     * @param file 文件名
     * @param line 行号
     * @param function 函数名
     */
    void info(const std::string & message, const char * file = "", unsigned int line = 0, const char * function = "");

    /**
     * @brief 记录警告日志
     * @param message 日志消息
     * @param file 文件名
     * @param line 行号
     * @param function 函数名
     */
    void warn(const std::string & message, const char * file = "", unsigned int line = 0, const char * function = "");

    /**
     * @brief 记录错误日志
     * @param message 日志消息
     * @param file 文件名
     * @param line 行号
     * @param function 函数名
     */
    void error(const std::string & message, const char * file = "", unsigned int line = 0, const char * function = "");

    /**
     * @brief 记录致命日志
     * @param message 日志消息
     * @param file 文件名
     * @param line 行号
     * @param function 函数名
     */
    void fatal(const std::string & message, const char * file = "", unsigned int line = 0, const char * function = "");

    /**
     * @brief 刷新日志输出器
     */
    void flush();

private:
    LogLevel level;                                 // 日志级别
    LogType type;                                   // 日志类型
    bool formatted;                                 // 是否格式化
    std::shared_ptr<LogFormatterBase> formatter;    // 日志格式化器
    std::unique_ptr<LoggerBase> logger;             // 日志记录器
    std::string name;                               // 日志名称
};

} // namespace vectordb::utils
