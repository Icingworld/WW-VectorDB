#pragma once

#include <vector>
#include <string>
#include <cstdint>

namespace WW
{

/**
 * @brief Serializer for vector
 */
class Serializer
{
public:
    /**
     * @brief serialize vector<float> to vector<char>
     */
    static std::vector<char> Serialize(const std::vector<float> & vector);

    /**
     * @brief deserialize vector<char> to vector<float>
     */
    static std::vector<float> Deserialize(const std::vector<char> & vector);
};

} // namespace WW
