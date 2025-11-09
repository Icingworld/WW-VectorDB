#include "vectordb/utils/logger/formatter/default_formatter.h"

#include <chrono>
#include <iomanip>

#include "vectordb/utils/logger/common/level.h"

namespace vectordb::utils
{

DefaultFormatter::DefaultFormatter()
{
    compilePattern("[%n][%c][%L] %v");
}

DefaultFormatter::DefaultFormatter(const std::string & pattern)
{
    compilePattern(pattern);
}

std::string DefaultFormatter::format(const LogMessage & message) const
{
    std::ostringstream os;
    std::tm tm_time{};
    bool timed = false;

    for (const auto & instruction : instructions) {
        instruction(os, message, tm_time, timed);
    }

    return os.str();
}

void DefaultFormatter::compilePattern(const std::string & pattern)
{
    for (std::size_t i = 0; i < pattern.size(); ++i) {
        if (pattern[i] == '%' && i + 1 < pattern.size()) {
            char code = pattern[i + 1];
            switch (code) {
                case 'Y':
                case 'y':
                case 'b':
                case 'h':
                case 'B':
                case 'm':
                case 'd':
                case 'e':
                case 'a':
                case 'A':
                case 'w':
                case 'u':
                case 'H':
                case 'I':
                case 'M':
                case 'S':
                case 'c':
                case 'D':
                case 'F':
                case 'T':
                case 'P':
                case 'Z':
                    instructions.emplace_back([this, code](
                        std::ostringstream & oss,
                        const LogMessage & msg,
                        std::tm & tm_time,
                        bool & timed
                    ) {
                        if (!timed) {
                            getTime(msg, tm_time);
                            timed = true;
                        }
                        char fmt_buf[3] = { '%', code, '\0' };
                        oss << std::put_time(&tm_time, fmt_buf);
                    });
                    break;
                case 'L':
                    instructions.emplace_back([](
                        std::ostringstream & oss,
                        const LogMessage & msg,
                        std::tm &,
                        bool &
                    ) {
                        oss << toString(msg.level);
                    });
                    break;
                case 't':
                    instructions.emplace_back([](
                        std::ostringstream & oss,
                        const LogMessage & msg,
                        std::tm &,
                        bool &
                    ) {
                        oss << msg.thread_id;
                    });
                    break;
                case 'f':
                    instructions.emplace_back([](
                        std::ostringstream & oss,
                        const LogMessage & msg,
                        std::tm &,
                        bool &
                    ) {
                        oss << msg.file;
                    });
                    break;
                case 'l':
                    instructions.emplace_back([](
                        std::ostringstream & oss,
                        const LogMessage & msg,
                        std::tm &,
                        bool &
                    ) {
                        oss << msg.line;
                    });
                    break;
                case 'C':
                    instructions.emplace_back([](
                        std::ostringstream & oss,
                        const LogMessage & msg,
                        std::tm &,
                        bool &
                    ) {
                        oss << msg.function;
                    });
                    break;
                case 'V':
                    instructions.emplace_back([](
                        std::ostringstream & oss,
                        const LogMessage & msg,
                        std::tm &,
                        bool &
                    ) {
                        oss << msg.file << ":" << msg.line << "-" << msg.function;
                    });
                    break;
                case 'n':
                    instructions.emplace_back([](
                        std::ostringstream & oss,
                        const LogMessage & msg,
                        std::tm &,
                        bool &
                    ) {
                        oss << msg.name;
                    });
                    break;
                case 'v':
                    instructions.emplace_back([](
                        std::ostringstream & oss,
                        const LogMessage & msg,
                        std::tm &,
                        bool &
                    ) {
                        oss << msg.message;
                    });
                    break;
                default:
                    instructions.emplace_back([code](
                        std::ostringstream & oss,
                        const LogMessage &,
                        std::tm &,
                        bool &
                    ) {
                        oss << '%' << code;
                    });
            }
            ++i;
        } else {
            size_t start = i;
            while (i < pattern.size() && !(pattern[i] == '%' && i + 1 < pattern.size())) {
                ++i;
            }
            std::string literal = pattern.substr(start, i - start);
            instructions.emplace_back([literal](
                std::ostringstream& oss,
                const LogMessage&,
                std::tm &,
                bool &
            ) {
                oss << literal;
            });
            --i;
        }
    }
}

void DefaultFormatter::getTime(const LogMessage & message, std::tm & tm_time) const
{
    time_t time = std::chrono::system_clock::to_time_t(message.timestamp);
    #ifdef _WIN32
        localtime_s(&tm_time, &time);   // Windows
    #else
        localtime_r(&time, &tm_time);   // Linux/Mac
    #endif
}

} // namespace vectordb::utils
