#include "vectordb/utils/logger/logger/base.h"

#include "vectordb/utils/logger/sink/base.h"
#include "vectordb/utils/logger/formatter/base.h"

namespace vectordb::utils
{

LoggerBase::LoggerBase(LogType type)
    : type(type)
{
}

LoggerBase::~LoggerBase() = default;

void LoggerBase::addSink(std::shared_ptr<LogSinkBase> sink)
{
    sinks.emplace_back(std::move(sink));
}

LogType LoggerBase::getType() const
{
    return type;
}

void LoggerBase::setFormatter(const std::string & pattern)
{
    for (auto & sink : sinks) {
        sink->setFormatter(pattern);
    }
}

void LoggerBase::setFormatter(std::shared_ptr<LogFormatterBase> formatter)
{
    for (auto & sink : sinks) {
        sink->setFormatter(formatter);
    }
}

} // namespace vectordb::utils
