#include "Collection.h"

#include <algorithm>

namespace WW
{

Collection::Collection(const CollectionMeta & meta)
    : meta_(meta)
    , segments_()
{
}

Collection::Collection(const std::string & id, const std::string & name, int dimension,
    MetricType metric, IndexType index, std::size_t max_rows, const std::string & description)
    : meta_()
    , segments_()
{
    meta_.id = id;
    meta_.name = name;
    meta_.dimension = dimension;
    meta_.metric_type = metric;
    meta_.index_type = index;
    meta_.segment_max_rows = max_rows;
    meta_.description = description;
}

const std::string & Collection::Id() const
{
    return meta_.id;
}

const std::string& Collection::Name() const
{
    return meta_.name;
}

int Collection::Dimension() const
{
    return meta_.dimension;
}

MetricType Collection::Metric() const
{
    return meta_.metric_type;
}

IndexType Collection::Index() const
{
    return meta_.index_type;
}

std::size_t Collection::SegmentMaxRows() const
{
    return meta_.segment_max_rows;
}

const std::string& Collection::Description() const
{
    return meta_.description;
}

bool Collection::AddSegment(const std::shared_ptr<Segment>& segment)
{
    if (!segment) {
        segments_.emplace_back(segment);
        return true;
    }

    return false;
}

bool Collection::RemoveSegment(const std::string & id)
{
    auto it = std::remove_if(segments_.begin(), segments_.end(), [&](const std::shared_ptr<Segment> & s) {
        s->Id() == id;
    });
    if (it != segments_.end()) {
        // TODO
        return true;
    }

    return false;
}

std::shared_ptr<Segment> Collection::GetSegment(const std::string & id) const
{
    for (const auto & segment : segments_) {
        if (segment->Id() == id) {
            return segment;
        }
    }

    return nullptr;
}

const std::vector<std::shared_ptr<Segment>> & Collection::Segments() const
{
    return segments_;
}

} // namespace WW
