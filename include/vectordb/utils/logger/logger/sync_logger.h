#pragma once

#include <mutex>

#include "vectordb/utils/logger/logger/base.h"

namespace vectordb::utils
{

/**
 * @brief 同步日志记录器
 */
class SyncLogger final : public LoggerBase
{
public:
    SyncLogger() = delete;

    explicit SyncLogger(LogType type);

    ~SyncLogger() override = default;

public:
    /**
     * @brief 记录日志
     * @param name 日志名称
     * @param level 日志级别
     * @param message 日志消息
     * @param file 文件名
     * @param line 行号
     * @param function 函数名
     */
    void log(const std::string & name, LogLevel level, const std::string & message, const char * file = "", unsigned int line = 0, const char * function = "") override;

    /**
     * @brief 刷新日志
     */
    void flush() override;

private:
    std::mutex mutex;
};

} // namespace vectordb::utils