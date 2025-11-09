#include "vectordb/utils/logger/logger/sync_logger.h"

#include <chrono>
#include <thread>

#include "vectordb/utils/logger/common/message.h"
#include "vectordb/utils/logger/sink/base.h"

namespace vectordb::utils
{

SyncLogger::SyncLogger(LogType type)
    : LoggerBase(type)
{
}

void SyncLogger::log(const std::string & name, LogLevel level, const std::string & message, const char * file, unsigned int line, const char * function)
{
    std::lock_guard<std::mutex> lock(mutex);

    LogMessage log_message;
    log_message.name = name;
    log_message.level = level;
    log_message.message = message;
    log_message.file = file;
    log_message.line = line;
    log_message.function = function;
    log_message.timestamp = std::chrono::system_clock::now();
    log_message.thread_id = std::this_thread::get_id();

    for (const auto & sink : sinks) {
        sink->write(log_message);
    }
}

void SyncLogger::flush()
{
    std::lock_guard<std::mutex> lock(mutex);
    for (const auto & sink : sinks) {
        sink->flush();
    }
}

} // namespace vectordb::utils
