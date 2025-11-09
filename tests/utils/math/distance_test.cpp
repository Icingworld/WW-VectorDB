#include <cmath>
#include <iostream>
#include <vector>

#include "vectordb/utils/math/distance.h"

using vectordb::utils::Distance;

int main()
{
    const std::vector<float> v1 { 1.0f, 2.0f, 3.0f };
    const std::vector<float> v2 { 4.0f, 5.0f, 6.0f };

    float squared = Distance::l2DistanceSquared(v1, v2);
    if (std::abs(squared - 27.0f) > 1e-5f) {
        std::cerr << "Unexpected L2 squared: " << squared << std::endl;
        return 1;
    }

    float dist = Distance::l2Distance(v1, v2);
    if (std::abs(dist - std::sqrt(27.0f)) > 1e-5f) {
        std::cerr << "Unexpected L2 distance: " << dist << std::endl;
        return 1;
    }

    try {
        const std::vector<float> shorter { 1.0f, 2.0f };
        Distance::l2DistanceSquared(shorter, v2);
        std::cerr << "Expected exception for size mismatch" << std::endl;
        return 1;
    } catch (const std::invalid_argument &) {
    }

    const std::vector<double> v3 { 1.0, 0.0, -1.0 };
    const std::vector<double> v4 { -1.0, 0.0, 1.0 };

    double cos_sim = Distance::cosineSimilarity(v3, v4);
    if (std::abs(cos_sim + 1.0) > 1e-12) {
        std::cerr << "Unexpected cosine similarity: " << cos_sim << std::endl;
        return 1;
    }

    const std::vector<float> zero { 0.0f, 0.0f, 0.0f };
    try {
        (void)Distance::cosineSimilarity(v1, zero);
        std::cerr << "Expected exception for zero vector" << std::endl;
        return 1;
    } catch (const std::invalid_argument &) {
    }

    return 0;
}

