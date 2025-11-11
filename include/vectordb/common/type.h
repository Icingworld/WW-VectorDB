#pragma once

#include <cstdint>

namespace vectordb::common
{

/**
 * @brief 距离指标类型
 */
enum class MetricType : std::uint8_t
{
    L2 = 0,      // L2 距离
    IP = 1,      // IP 距离
    COS = 2,     // COS 距离
};

/**
 * @brief 索引类型
 */
enum class IndexType : std::uint8_t
{
    FLAT = 0,      // 平铺索引
    IVF = 1,       // IVF 索引
    HNSW = 2,      // HNSW 索引
};

/**
 * @brief 数据类型
 */
enum class DataType : std::uint8_t
{
    FLOAT = 0,      // 浮点数
    DOUBLE = 1,     // 双精度浮点数
    INT8 = 2,       // 8 位整数
    INT16 = 3,      // 16 位整数
    INT32 = 4,      // 32 位整数
    INT64 = 5,      // 64 位整数
};

} // namespace vectordb::common
