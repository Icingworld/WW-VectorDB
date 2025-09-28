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
     * @brief serialize vector to string
     * @details [uint32 dim][float...]
     */
    static std::string Serialize(const std::vector<float> & vec);

    /**
     * @brief deserialize string to vector
     */
    static std::vector<float> Deserialize(const std::string & blob);

private:
    /**
     * @brief write uint32 to little-endian
     */
    static void WriteUint32LE(char * dst, uint32_t value);

    /**
     * @brief read uint32 from little endian
     */
    static uint32_t ReadUint32LE(const char * src);
};

} // namespace WW
