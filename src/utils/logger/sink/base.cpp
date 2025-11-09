#include "vectordb/utils/logger/sink/base.h"

#include "vectordb/utils/logger/formatter/base.h"
#include "vectordb/utils/logger/formatter/default_formatter.h"

namespace vectordb::utils
{

LogSinkBase::LogSinkBase()
    : formatter(std::make_shared<DefaultFormatter>())
{
}

LogSinkBase::LogSinkBase(const std::string & pattern)
    : formatter(std::make_shared<DefaultFormatter>(pattern))
{
}

LogSinkBase::LogSinkBase(std::shared_ptr<LogFormatterBase> formatter)
    : formatter(formatter)
{
}

LogSinkBase::~LogSinkBase() = default;

void LogSinkBase::setFormatter(std::string pattern)
{
    formatter = std::make_shared<DefaultFormatter>(pattern);
}

void LogSinkBase::setFormatter(std::shared_ptr<LogFormatterBase> formatter)
{
    this->formatter = formatter;
}

} // namespace vectordb::utils
