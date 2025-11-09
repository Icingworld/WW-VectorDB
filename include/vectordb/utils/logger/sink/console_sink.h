#pragma once

#include <memory>

#include "vectordb/utils/logger/sink/base.h"

namespace vectordb::utils
{

class LogFormatterBase;

/**
 * @brief 控制台日志输出器
 */
class ConsoleSink final : public LogSinkBase
{
public:
    ConsoleSink();

    explicit ConsoleSink(const std::string & pattern);

    explicit ConsoleSink(std::shared_ptr<LogFormatterBase> formatter);

    ~ConsoleSink() override = default;

public:
    /**
     * @brief 写入日志
     * @param message 日志消息
     */
    void write(const LogMessage & message) override;

    /**
     * @brief 刷新日志
     */
    void flush() override;
};

} // namespace vectordb::utils