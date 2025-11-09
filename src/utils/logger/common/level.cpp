#include "vectordb/utils/logger/common/level.h"

#include <stdexcept>

namespace vectordb::utils
{

std::string toString(LogLevel level)
{
    switch (level) {
        case LogLevel::DEBUG:
            return "DEBUG";
        case LogLevel::INFO:
            return "INFO";
        case LogLevel::WARN:
            return "WARN";
        case LogLevel::ERROR:
            return "ERROR";
        case LogLevel::FATAL:
            return "FATAL";
        default:
            return "UNKNOWN";
    }
}

LogLevel fromString(const std::string & str)
{
    if (str == "DEBUG") {
        return LogLevel::DEBUG;
    } else if (str == "INFO") {
        return LogLevel::INFO;
    } else if (str == "WARN") {
        return LogLevel::WARN;
    } else if (str == "ERROR") {
        return LogLevel::ERROR;
    } else if (str == "FATAL") {
        return LogLevel::FATAL;
    } else {
        throw std::invalid_argument("Invalid log level: " + str);
    }
}

} // namespace vectordb::utils