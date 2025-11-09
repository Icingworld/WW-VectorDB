#pragma once

#include <cmath>
#include <stdexcept>
#include <type_traits>

#include "vectordb/utils/math/distance.h"

namespace vectordb::utils
{

inline void Distance::ensureSameSize(std::size_t left, std::size_t right)
{
    if (left != right) {
        throw std::invalid_argument("distance: vector size mismatch");
    }
}

template <typename T>
void Distance::ensurePointerValid(const T * ptr, std::size_t size)
{
    static_assert(std::is_floating_point_v<T>, "Distance expects floating-point types");
    if (ptr == nullptr && size > 0) {
        throw std::invalid_argument("distance: null pointer input");
    }
}

template <typename T>
T Distance::l2DistanceSquared(const T * vec1, const T * vec2, std::size_t size)
{
    ensurePointerValid(vec1, size);
    ensurePointerValid(vec2, size);

    T sum = T { 0 };
    for (std::size_t i = 0; i < size; ++i) {
        T diff = vec1[i] - vec2[i];
        sum += diff * diff;
    }
    return sum;
}

template <
    typename Container,
    typename Value,
    typename Enable
>
Value Distance::l2DistanceSquared(const Container & vec1, const Container & vec2)
{
    ensureSameSize(vec1.size(), vec2.size());
    return l2DistanceSquared<Value>(vec1.data(), vec2.data(), vec1.size());
}

template <typename T>
T Distance::l2Distance(const T * vec1, const T * vec2, std::size_t size)
{
    return std::sqrt(l2DistanceSquared(vec1, vec2, size));
}

template <
    typename Container,
    typename Value,
    typename Enable
>
Value Distance::l2Distance(const Container & vec1, const Container & vec2)
{
    ensureSameSize(vec1.size(), vec2.size());
    return l2Distance<Value>(vec1.data(), vec2.data(), vec1.size());
}

template <typename T>
T Distance::cosineSimilarity(const T * vec1, const T * vec2, std::size_t size)
{
    ensurePointerValid(vec1, size);
    ensurePointerValid(vec2, size);

    if (size == 0) {
        throw std::invalid_argument("distance: cosine similarity on empty vectors");
    }

    T dot = T { 0 };
    T norm_a = T { 0 };
    T norm_b = T { 0 };

    for (std::size_t i = 0; i < size; ++i) {
        dot += vec1[i] * vec2[i];
        norm_a += vec1[i] * vec1[i];
        norm_b += vec2[i] * vec2[i];
    }

    if (norm_a == T { 0 } || norm_b == T { 0 }) {
        throw std::invalid_argument("distance: cosine similarity with zero vector");
    }

    return dot / (std::sqrt(norm_a) * std::sqrt(norm_b));
}

template <
    typename Container,
    typename Value,
    typename Enable
>
Value Distance::cosineSimilarity(const Container & vec1, const Container & vec2)
{
    ensureSameSize(vec1.size(), vec2.size());
    return cosineSimilarity<Value>(vec1.data(), vec2.data(), vec1.size());
}

} // namespace vectordb::utils
