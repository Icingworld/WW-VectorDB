#pragma once

#include <string>
#include <cstddef>

namespace WW
{

/**
 * @brief type of metric
 */
enum class MetricType : uint8_t
{
    L2,
    COSINE
};

/**
 * @brief type of index
 */
enum class IndexType : uint8_t
{
    BRUTEFORCE,
    IVF,
    PQ,
    IVFPQ,
    HNSW
};

/**
 * @brief convert metric type to string
 */
inline std::string MetricTypeToString(MetricType metric);

/**
 * @brief generate metric type from string
 */
inline MetricType MetricTypeFromString(const std::string & str);

/**
 * @brief convert index type to string
 */
inline std::string IndexTypeToString(IndexType type);

/**
 * @brief generate index type from string
 */
inline IndexType IndexTypeFromString(const std::string & str);

} // namespace WW
