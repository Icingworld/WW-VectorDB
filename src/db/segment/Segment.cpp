#include "Segment.h"

#include <cstring>

#include "Serializer.h"

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
    std::vector<char> ids;
    ids.reserve(data_.size() * sizeof(int64_t));

    std::vector<char> vectors;
    vectors.reserve(data_.size() * meta_.dimension * sizeof(float));

    for (auto & [id, vector] : data_) {
        const char * id_ptr = reinterpret_cast<const char *>(&id);
        ids.insert(ids.end(), id_ptr, id_ptr + sizeof(int64_t));

        const char * vec_ptr = reinterpret_cast<const char *>(vector.data());
        vectors.insert(vectors.end(), vec_ptr, vec_ptr + vector.size() * sizeof(float));
    }

    fs_->WriteFile(meta_.path + "/id.bin", ids);
    fs_->WriteFile(meta_.path + "/vector.bin", vectors);
}

void Segment::Load()
{
    std::vector<char> ids = fs_->ReadFile(meta_.path + "/id.bin");
    std::vector<char> vectors = fs_->ReadFile(meta_.path + "/vector.bin");

    size_t row_count = ids.size() / sizeof(int64_t);
    data_.clear();
    data_.reserve(row_count);

    const int64_t * id_ptr = reinterpret_cast<const int64_t *>(ids.data());
    const float * vec_ptr  = reinterpret_cast<const float *>(vectors.data());

    for (size_t i = 0; i < row_count; i++) {
        int64_t id = id_ptr[i];
        std::vector<float> vec(meta_.dimension);

        std::memcpy(vec.data(), vec_ptr + i * meta_.dimension, meta_.dimension * sizeof(float));

        data_.emplace(id, std::move(vec));
    }

    meta_.row_count = row_count;
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
