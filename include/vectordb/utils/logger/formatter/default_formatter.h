#pragma once

#include <vector>
#include <functional>
#include <sstream>
#include <ctime>

#include "vectordb/utils/logger/formatter/base.h"

namespace vectordb::utils
{

/**
 * @brief 默认日志格式化器
 */
class DefaultFormatter final : public LogFormatterBase
{
public:
    DefaultFormatter();

    explicit DefaultFormatter(const std::string & pattern);

    ~DefaultFormatter() override = default;

    std::string format(const LogMessage & message) const override;

private:
    /**
     * @brief 预编译格式化模式
     * @param pattern 格式化模式
     */
    void compilePattern(const std::string & pattern);

    /**
     * @brief 获取时间
     * @param message 日志消息
     * @param tm_time 时间
     */
    void getTime(const LogMessage & message, std::tm & tm_time) const;

private:
    std::vector<
        std::function<void(
            std::ostringstream & os,
            const LogMessage & message,
            std::tm & tm_time,
            bool & timed
        )>
    > instructions; // 指令集
};

} // namespace vectordb::utils
