#pragma once

#include <memory>
#include <string>

#include "vectordb/utils/logger/common/message.h"

namespace vectordb::utils
{

class LogFormatterBase;

/**
 * @brief 日志输出器基类
 */
class LogSinkBase
{
public:
    LogSinkBase();

    explicit LogSinkBase(const std::string & pattern);

    explicit LogSinkBase(std::shared_ptr<LogFormatterBase> formatter);

    virtual ~LogSinkBase();

public:
    /**
     * @brief 写入日志
     * @param message 日志消息
     */
    virtual void write(const LogMessage & message) = 0;

    /**
     * @brief 刷新日志
     */
    virtual void flush() = 0;

    /**
     * @brief 设置日志格式化器
     * @param pattern 日志格式化器模式
     * @details 使用默认日志格式化器
     */
    void setFormatter(std::string pattern);

    /**
     * @brief 设置日志格式化器
     * @param formatter 日志格式化器
     */
    void setFormatter(std::shared_ptr<LogFormatterBase> formatter);

protected:
    std::shared_ptr<LogFormatterBase> formatter;
};

} // namespace vectordb::utils
