#include "vectordb/utils/logger/sink/console_sink.h"

#include <iostream>

#include "vectordb/utils/logger/formatter/base.h"

namespace vectordb::utils
{

ConsoleSink::ConsoleSink()
    : LogSinkBase()
{
}

ConsoleSink::ConsoleSink(const std::string & pattern)
    : LogSinkBase(pattern)
{
}

ConsoleSink::ConsoleSink(std::shared_ptr<LogFormatterBase> formatter)
    : LogSinkBase(formatter)
{
}

void ConsoleSink::write(const LogMessage & message)
{
    if (message.level != LogLevel::ERROR && message.level != LogLevel::FATAL) {
        std::cout << formatter->format(message) << std::endl;
    } else {
        std::cerr << formatter->format(message) << std::endl;
    }
}

void ConsoleSink::flush()
{
    std::cout.flush();
    std::cerr.flush();
}

} // namespace vectordb::utils
