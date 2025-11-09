#pragma once

#include <string>
#include <chrono>
#include <thread>
#include <cstdint>

#include "vectordb/utils/logger/common/level.h"

namespace vectordb::utils
{

/**
 * @brief 日志消息
 */
class LogMessage final
{
public:
    LogMessage() = default;

    ~LogMessage() = default;

public:
    std::string name;                                   // 日志名称
    LogLevel level;                                     // 日志等级
    std::string message;                                // 日志消息
    std::string file;                                   // 文件名
    std::uint32_t line;                                 // 行号
    std::string function;                               // 函数名
    std::chrono::system_clock::time_point timestamp;    // 时间戳
    std::thread::id thread_id;                          // 线程ID
};

} // namespace vectordb::utils
