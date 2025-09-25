#pragma once

#include <vector>

namespace WW
{

/**
 * @brief Utility class for vector operations
 */
class VectorUtils
{
public:
    /**
     * @brief Compute L2 norm of a vector
     * @param a Vector
     * @return L2 norm (non-negative)
     */
    [[nodiscard]] static float L2Norm(const std::vector<float> & a);

    /**
     * @brief Compute dot product between two vectors
     * @param a First vector
     * @param b Second vector
     * @return Dot product
     */
    [[nodiscard]] static float Dot(const std::vector<float> & a, const std::vector<float> & b);

    /**
     * @brief Compute cosine similarity between two vectors
     * @param a First vector
     * @param b Second vector
     * @return Cosine similarity in [-1, 1]
     */
    [[nodiscard]] static float CosineSimilarity(const std::vector<float> & a, const std::vector<float> & b);

    /**
     * @brief Compute squared L2 (Euclidean) distance
     * @param a First vector
     * @param b Second vector
     * @return Squared L2 distance (non-negative)
     */
    [[nodiscard]] static float L2DistanceSquared(const std::vector<float> & a, const std::vector<float> & b);

    /**
     * @brief Compute L2 distance
     * @param a First vector
     * @param b Second vector
     * @return L2 distance (non-negative)
     */
    [[nodiscard]] static float L2Distance(const std::vector<float> & a, const std::vector<float> & b);
};

} // namespace WW
