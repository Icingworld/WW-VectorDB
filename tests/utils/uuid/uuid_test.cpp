#include <algorithm>
#include <array>
#include <iostream>
#include <stdexcept>
#include <unordered_set>
#include <vector>

#include "vectordb/utils/uuid.h"

using vectordb::utils::NoSeperatorUUID;
using vectordb::utils::UUID;

namespace
{

bool expectThrowsInvalidArgument(const std::string & input)
{
    try {
        (void)UUID::fromString(input);
    } catch (const std::invalid_argument &) {
        return true;
    } catch (...) {
    }
    return false;
}

} // namespace

int main()
{
    UUID uuid = UUID::create();
    const auto & bytes = uuid.bytes();
    const std::string uuid_str = uuid.toString();

    if (uuid_str.size() != 36) {
        std::cerr << "UUID string length mismatch: " << uuid_str << std::endl;
        return 1;
    }

    const std::array<int, 4> hyphen_positions { 8, 13, 18, 23 };
    for (int pos : hyphen_positions) {
        if (uuid_str[pos] != '-') {
            std::cerr << "Expected hyphen at position " << pos << " in " << uuid_str << std::endl;
            return 1;
        }
    }

    if ((bytes[6] >> 4) != 0x04) {
        std::cerr << "Version bits incorrect: " << static_cast<int>(bytes[6]) << std::endl;
        return 1;
    }

    if ((bytes[8] & 0xC0) != 0x80) {
        std::cerr << "Variant bits incorrect: " << static_cast<int>(bytes[8]) << std::endl;
        return 1;
    }

    UUID parsed = UUID::fromString(uuid_str);
    if (parsed != uuid) {
        std::cerr << "Parsed UUID does not match original" << std::endl;
        return 1;
    }

    NoSeperatorUUID compact;
    std::string compact_str = compact.toString();
    if (compact_str.size() != 32 || compact_str.find('-') != std::string::npos) {
        std::cerr << "Compact UUID format incorrect: " << compact_str << std::endl;
        return 1;
    }

    UUID parsed_compact = UUID::fromString(compact_str);
    if (parsed_compact.bytes() != compact.bytes()) {
        std::cerr << "Parsing compact UUID failed" << std::endl;
        return 1;
    }

    if (!expectThrowsInvalidArgument("invalid-uuid")) {
        std::cerr << "Expected invalid_argument for malformed UUID" << std::endl;
        return 1;
    }

    std::vector<UUID> uuids;
    for (int i = 0; i < 10; ++i) {
        uuids.emplace_back(UUID::create());
    }
    std::vector<UUID> sorted = uuids;
    std::sort(sorted.begin(), sorted.end());

    std::unordered_set<UUID> unique;
    for (const auto & id : uuids) {
        if (!unique.insert(id).second) {
            std::cerr << "Duplicate UUID generated" << std::endl;
            return 1;
        }
    }

    if (!std::is_sorted(sorted.begin(), sorted.end())) {
        std::cerr << "UUID ordering inconsistent" << std::endl;
        return 1;
    }

    return 0;
}

