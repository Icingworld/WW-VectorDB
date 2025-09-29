#include "Serializer.h"

#include <cstring>
#include <stdexcept>

namespace WW
{

std::vector<char> Serializer::Serialize(const std::vector<float> & vector)
{
    std::vector<char> buffer(vector.size() * sizeof(float));
    std::memcpy(buffer.data(), vector.data(), buffer.size());
    return buffer;
}

std::vector<float> Serializer::Deserialize(const std::vector<char> & data)
{
    size_t n = data.size() / sizeof(float);
    std::vector<float> vec(n);
    if (n > 0) {
        std::memcpy(vec.data(), data.data(), data.size());
    }
    return vec;
}

} // namespace WW
