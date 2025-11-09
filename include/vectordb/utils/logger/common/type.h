#pragma once

#include <cstdint>

namespace vectordb::utils
{

enum class LogType : std::uint8_t
{
    SYNC = 0,       // 同步
    ASYNC = 1,      // 异步
};

} // namespace vectordb::utils
