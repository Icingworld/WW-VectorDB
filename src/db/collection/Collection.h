#pragma once

#include <string>
#include <vector>
#include <cstddef>
#include <memory>

#include "Types.h"
#include "Segment.h"

namespace WW
{

/**
 * @brief meta data of collection
 */
struct CollectionMeta
{
    std::string id;                 // UUID
    std::string name;               // name of collection
    int dimention;                  // dimension of collection
    MetricType metric_type;         // type of metric
    IndexType index_type;           // type of index
    std::size_t segment_max_rows;   // max rows per segment
    std::string description;        // notes
};

class Collection
{
public:

private:
    CollectionMeta meta_;
    std::vector<std::shared_ptr<Segment>> segments_;
};

} // namespace WW
