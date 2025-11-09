#include <memory>
#include <string>

#include "vectordb/utils/logger/logger.h"
#include "vectordb/utils/logger/sink/console_sink.h"

using vectordb::utils::LogLevel;
using vectordb::utils::Logger;
using vectordb::utils::ConsoleSink;

int main()
{
    auto sink = std::make_shared<ConsoleSink>();

    auto & logger = Logger::getSyncLogger("sync_console_logger_test");
    logger.addSink(sink);
    logger.setLevel(LogLevel::WARN);

    logger.log(LogLevel::INFO, "info message");
    logger.log(LogLevel::WARN, "warn message", __FILE__, __LINE__, __func__);
    logger.log(LogLevel::ERROR, "error message");
    logger.flush();

    return 0;
}
