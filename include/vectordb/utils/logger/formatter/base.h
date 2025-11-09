#pragma once

#include "vectordb/utils/logger/common/message.h"

namespace vectordb::utils
{

/**
 * @brief 日志格式化器基类
 */
class LogFormatterBase
{
public:
    LogFormatterBase() = default;

    virtual ~LogFormatterBase() = default;

public:
    /**
     * @brief 格式化日志消息
     * @param message 日志消息
     * @return 格式化后的日志消息
     */
    virtual std::string format(const LogMessage & message) const = 0;
};

} // namespace vectordb::utils
