#pragma once

#include <cstdint>
#include <string>

namespace vectordb::utils
{

/**
 * @brief 日志级别
 */
enum class LogLevel : std::uint8_t
{
    DEBUG = 0,      // 调试
    INFO = 1,       // 信息
    WARN = 2,       // 警告
    ERROR = 3,      // 错误
    FATAL = 4,      // 致命
};

/**
 * @brief 将日志级别转换为字符串
 * @param level 日志级别
 * @return 字符串
 */
std::string toString(LogLevel level);

/**
 * @brief 将字符串转换为日志级别
 * @param str 字符串
 * @return 日志级别
 */
LogLevel fromString(const std::string & str);

} // namespace vectordb::utils
