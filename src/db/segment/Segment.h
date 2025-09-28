#pragma once

#include <string>
#include <cstddef>

namespace WW
{

/**
 * @brief meta data of segment
 */
struct SegmentMeta
{
    std::string id;                 // segment id
    std::size_t row_count;          // current count
    std::size_t deleted_count;      // tombstone count
    bool is_sealed;                 // whether segment is sealed
    std::string path;               // path of storage
};

class Segment
{

};

} // namespace WW
