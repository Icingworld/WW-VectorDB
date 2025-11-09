#pragma once

#include <string>
#include <vector>
#include <memory>

#include "vectordb/utils/logger/common/type.h"
#include "vectordb/utils/logger/common/level.h"

namespace vectordb::utils
{

class LogSinkBase;
class LogFormatterBase;

/**
 * @brief 日志记录器基类
 */
class LoggerBase
{
public:
    LoggerBase() = delete;

    explicit LoggerBase(LogType type);

    virtual ~LoggerBase();

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
    virtual void log(const std::string & name, LogLevel level, const std::string & message, const char * file = "", unsigned int line = 0, const char * function = "") = 0;

    /**
     * @brief 刷新日志
     */
    virtual void flush() = 0;

    /**
     * @brief 添加日志输出器
     * @param sink 日志输出器
     */
    void addSink(std::shared_ptr<LogSinkBase> sink);
    
    /**
     * @brief 设置日志格式化器模式
     * @param pattern 日志格式化器模式
     * @details 使用默认日志格式化器
     */
    void setFormatter(const std::string & pattern);

    /**
     * @brief 设置日志格式化器
     * @param formatter 日志格式化器
     */
    void setFormatter(std::shared_ptr<LogFormatterBase> formatter);

    /**
     * @brief 获取日志类型
     * @return 日志类型
     */
    LogType getType() const;

protected:
    LogType type;
    std::vector<std::shared_ptr<LogSinkBase>> sinks;
};

} // namespace vectordb::utils
