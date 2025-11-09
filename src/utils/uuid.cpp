#include "vectordb/utils/uuid.h"

#include <array>
#include <cctype>
#include <iomanip>
#include <random>
#include <sstream>
#include <stdexcept>

namespace vectordb::utils
{

namespace
{

std::mt19937 makeEngine()
{
    std::random_device rd;
    std::seed_seq seed {
        rd(), rd(), rd(), rd(), rd(), rd(), rd(), rd()
    };
    return std::mt19937(seed);
}

std::mt19937 & threadEngine()
{
    thread_local std::mt19937 engine = makeEngine();
    return engine;
}

unsigned char randomByte()
{
    thread_local std::uniform_int_distribution<int> dist(0, 255);
    return static_cast<unsigned char>(dist(threadEngine()));
}

int hexValue(char ch)
{
    if (ch >= '0' && ch <= '9') {
        return ch - '0';
    }
    ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
    if (ch >= 'a' && ch <= 'f') {
        return ch - 'a' + 10;
    }
    throw std::invalid_argument("UUID contains non-hex characters");
}

} // namespace

UUID::UUID()
{
    generate();
}

UUID UUID::create()
{
    return UUID();
}

UUID UUID::fromString(const std::string & str)
{
    std::string compact;
    compact.reserve(32);

    for (char ch : str) {
        if (ch == '-') {
            continue;
        }
        if (!std::isxdigit(static_cast<unsigned char>(ch))) {
            throw std::invalid_argument("UUID contains non-hex characters");
        }
        compact.push_back(ch);
    }

    if (compact.size() != 32) {
        throw std::invalid_argument("UUID string must contain 32 hex digits");
    }

    UUID uuid;
    for (std::size_t i = 0; i < 16; ++i) {
        int hi = hexValue(compact[2 * i]);
        int lo = hexValue(compact[2 * i + 1]);
        uuid.uuid[i] = static_cast<unsigned char>((hi << 4) | lo);
    }
    return uuid;
}

void UUID::generate()
{
    for (auto & byte : uuid) {
        byte = randomByte();
    }

    uuid[6] = (uuid[6] & 0x0F) | 0x40;  // 设置版本号为 4
    uuid[8] = (uuid[8] & 0x3F) | 0x80;  // 设置变体为 RFC4122
}

std::string UUID::toString() const
{
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (int i = 0; i < 16; ++i) {
        ss << std::setw(2) << static_cast<int>(uuid[i]);
        if (i == 3 || i == 5 || i == 7 || i == 9) {
            ss << "-";
        }
    }
    return ss.str();
}

bool UUID::operator==(const UUID & other) const
{
    return uuid == other.uuid;
}

bool UUID::operator!=(const UUID & other) const
{
    return !(*this == other);
}

bool UUID::operator<(const UUID & other) const
{
    return uuid < other.uuid;
}

const std::array<unsigned char, 16> & UUID::bytes() const
{
    return uuid;
}

NoSeperatorUUID::NoSeperatorUUID()
    : UUID()
{
}

std::string NoSeperatorUUID::toString() const
{
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (int i = 0; i < 16; ++i) {
        ss << std::setw(2) << static_cast<int>(uuid[i]);
    }
    return ss.str();
}

} // namespace vectordb::utils

namespace std
{

std::size_t hash<vectordb::utils::UUID>::operator()(const vectordb::utils::UUID & uuid) const noexcept
{
    const auto & bytes = uuid.bytes();
    const std::size_t fnv_offset_basis = 1469598103934665603ULL;
    const std::size_t fnv_prime = 1099511628211ULL;

    std::size_t hash = fnv_offset_basis;
    for (unsigned char byte : bytes) {
        hash ^= static_cast<std::size_t>(byte);
        hash *= fnv_prime;
    }
    return hash;
}

} // namespace std

