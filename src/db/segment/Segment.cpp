#include "Segment.h"

namespace WW
{
    
Segment::Segment(const SegmentMeta & meta)
{
}

const std::string & Segment::Id() const
{
    return meta_.id;
}

const std::string & Segment::CollectionId() const
{
    return meta_.collection_id;
}

std::size_t Segment::RowCount() const
{
    return meta_.row_count;
}

bool Segment::IsSealed() const
{
    return meta_.is_sealed;
}

const std::string & Segment::Path() const
{
    return meta_.path;
}

void Segment::Seal()
{

}

void Segment::Save()
{

}

void Segment::Load()
{

}

bool Segment::Upsert(const std::string & id, const std::vector<float> & vector)
{

}

bool Segment::Delete(const std::string & id)
{

}

std::optional<std::vector<float>> Segment::Get(const std::string & id) const
{

}

std::vector<SearchResult> Segment::Search(const std::vector<float> & query, int top_k) const
{

}

} // namespace WW
