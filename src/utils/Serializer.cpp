#include "Serializer.h"

#include <cstring>
#include <stdexcept>

namespace WW
{

std::string Serializer::Serialize(const std::vector<float> & vec)
{
    uint32_t dim = static_cast<uint32_t>(vec.size());
    std::string buffer;
    buffer.resize(sizeof(uint32_t) + dim * sizeof(float));

    WriteUint32LE(buffer.data(), dim);
    std::memcpy(buffer.data() + sizeof(uint32_t), vec.data(), dim * sizeof(float));

    return buffer;
}

std::vector<float> Serializer::Deserialize(const std::string & blob)
{
    if (blob.size() < sizeof(uint32_t)) {
        throw std::runtime_error("Invalid blob: too small");
    }

    uint32_t dim = ReadUint32LE(blob.data());
    size_t expected_size = sizeof(uint32_t) + dim * sizeof(float);

    if (blob.size() != expected_size) {
        throw std::runtime_error("Invalid blob: size mismatch");
    }

    std::vector<float> vec(dim);
    std::memcpy(vec.data(), blob.data() + sizeof(uint32_t), dim * sizeof(float));
    return vec;
}

void Serializer::WriteUint32LE(char * dst, uint32_t value)
{
    dst[0] = static_cast<char>(value & 0xFF);
    dst[1] = static_cast<char>((value >> 8) & 0xFF);
    dst[2] = static_cast<char>((value >> 16) & 0xFF);
    dst[3] = static_cast<char>((value >> 24) & 0xFF);
}

uint32_t Serializer::ReadUint32LE(const char * src)
{
    return (static_cast<uint8_t>(src[0]) |
            (static_cast<uint8_t>(src[1]) << 8) |
            (static_cast<uint8_t>(src[2]) << 16) |
            (static_cast<uint8_t>(src[3]) << 24));
}

} // namespace WW
