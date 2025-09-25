#include "VectorUtils.h"

#include <cassert>
#include <cmath>
#include <stdexcept>

namespace WW
{

float VectorUtils::L2Norm(const std::vector<float> & a)
{
    float res = 0.0f;
    for (float x : a) {
        res += x * x;
    }
    return std::sqrt(res);
}

float VectorUtils::Dot(const std::vector<float> & a, const std::vector<float> & b)
{
    assert(a.size() == b.size());
    float res = 0.0f;
    for (size_t i = 0; i < a.size(); ++i) {
        res += a[i] * b[i];
    }
    return res;
}

float VectorUtils::CosineSimilarity(const std::vector<float> & a, const std::vector<float> & b)
{
    assert(a.size() == b.size());
    float dot = Dot(a, b);
    float norm_a = L2Norm(a);
    float norm_b = L2Norm(b);
    if (norm_a == 0 || norm_b == 0) {
        throw std::invalid_argument("Zero-length vector in CosineSimilarity()");
    }
    return dot / (norm_a * norm_b);
}

float VectorUtils::L2DistanceSquared(const std::vector<float> & a, const std::vector<float> & b)
{
    assert(a.size() == b.size());
    float res = 0.0f;
    for (size_t i = 0; i < a.size(); ++i) {
        float diff = a[i] - b[i];
        res += diff * diff;
    }
    return res;
}

float VectorUtils::L2Distance(const std::vector<float> & a, const std::vector<float> & b)
{
    return std::sqrt(L2DistanceSquared(a, b));
}

} // namespace WW
