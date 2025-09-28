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
    int dimension;                  // dimension of collection
    MetricType metric_type;         // type of metric
    IndexType index_type;           // type of index
    std::size_t segment_max_rows;   // max rows per segment
    std::string description;        // notes
};

class Collection
{
public:
    explicit Collection(const CollectionMeta & meta);

    Collection(const std::string & id, const std::string & name, int dimension,
        MetricType metric, IndexType index, std::size_t max_rows, const std::string & description);

    const std::string & Id() const;

    const std::string & Name() const;

    int Dimension() const;

    MetricType Metric() const;

    IndexType Index() const;

    std::size_t SegmentMaxRows() const;

    const std::string & Description() const;

    bool AddSegment(const std::shared_ptr<Segment>& segment);

    bool RemoveSegment(const std::string & id);

    std::shared_ptr<Segment> GetSegment(const std::string & id) const;

    const std::vector<std::shared_ptr<Segment>> & Segments() const;

private:
    CollectionMeta meta_;
    std::vector<std::shared_ptr<Segment>> segments_;
};

} // namespace WW
