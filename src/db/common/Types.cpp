#include "Types.h"

#include <stdexcept>

namespace WW
{

std::string MetricTypeToString(MetricType metric)
{
    switch (metric)
    {
    case MetricType::L2:
        return "L2";
    case MetricType::COSINE:
        return "COSINE";
    default:
        return "UNKNOWN";
    }
}

MetricType MetricTypeFromString(const std::string & str)
{
    if (str == "L2") {
        return MetricType::L2;
    } else if (str == "COSINE") {
        return MetricType::COSINE;
    } else {
        throw std::invalid_argument("Unkown metric type: " + str);
    }
}

std::string IndexTypeToString(IndexType index)
{
    switch (index)
    {
    case IndexType::BRUTEFORCE:
        return "BRUTEFORCE";
    case IndexType::IVF:
        return "IVF";
    case IndexType::PQ:
        return "PQ";
    case IndexType::IVFPQ:
        return "IVF-PQ";
    case IndexType::HNSW:
        return "HNSW";
    default:
        return "UNKNOWN";
    }
}

IndexType IndexTypeFromString(const std::string & str)
{
    if (str == "BRUTEFORCE") {
        return IndexType::BRUTEFORCE;
    } else if (str == "IVF") {
        return IndexType::IVF;
    } else if (str == "PQ") {
        return IndexType::PQ;
    } else if (str == "IVF-PQ") {
        return IndexType::IVFPQ;
    } else if (str == "HNSW") {
        return IndexType::HNSW;
    } else {
        throw std::invalid_argument("Unkown index type: " + str);
    }
};

} // namespace WW
