#include "vectordb/core/segment.h"

#include <functional>

#include "vectordb/core/meta.inl"

namespace vectordb::core
{

common::Json SegmentMeta::toJson() const
{
    common::Json json;
    json["segment_id"] = segment_id;
    json["collection_id"] = collection_id;
    json["status"] = static_cast<std::uint8_t>(status);
    json["row_count"] = row_count;
    json["deleted_count"] = deleted_count;
    json["storage_path"] = storage_path;
    json["index_type"] = static_cast<std::uint8_t>(index_type);
    return json;
}

void SegmentMeta::fromJson(const common::Json & json)
{
    segment_id = json["segment_id"].get<std::string>();
    collection_id = json["collection_id"].get<std::string>();
    status = static_cast<SegmentStatus>(json["status"].get<std::uint8_t>());
    row_count = json["row_count"].get<std::size_t>();
    deleted_count = json["deleted_count"].get<std::size_t>();
    storage_path = json["storage_path"].get<std::string>();
    index_type = static_cast<common::IndexType>(json["index_type"].get<std::uint8_t>());
}

void Segment::loadSegment()
{
    // 1. 加载段元数据
    LoadMeta(meta, getStoragePath() + "/meta.json");
}

void Segment::saveSegment()
{
    SaveMeta(meta, getStoragePath() + "/meta.json");
}

void Segment::sealSegment()
{
    // TODO: 密封段
}

void Segment::removeSegment() {
    // TODO: 删除段
}

void Segment::insertVector(const std::string & vector_id, const std::vector<float> & vector)
{
    if (vector_index.find(vector_id) != vector_index.end()) {
        // TODO: 日志记录
        return;
    }

    vectors.emplace_back(vector_id, vector);
    vector_index[vector_id] = vectors.size() - 1;
    meta.row_count++;
}

void Segment::deleteVector(const std::string & vector_id)
{
    if (vector_index.find(vector_id) == vector_index.end()) {
        // TODO: 日志记录
        return;
    }

    vectors.erase(vectors.begin() + vector_index[vector_id]);
    vector_index.erase(vector_id);
    meta.row_count--;
}

std::optional<std::reference_wrapper<const std::vector<float>>> Segment::getVector(const std::string & vector_id) const
{
    if (vector_index.find(vector_id) == vector_index.end()) {
        return std::nullopt;
    }

    return std::ref(vectors.at(vector_index.at(vector_id)).second);
}

const SegmentMeta & Segment::getMeta() const
{
    return meta;
}

const std::string & Segment::getSegmentId() const
{
    return meta.segment_id;
}

const std::string & Segment::getCollectionId() const
{
    return meta.collection_id;
}

SegmentStatus Segment::getStatus() const
{
    return meta.status;
}

std::size_t Segment::getRowCount() const
{
    return meta.row_count;
}

std::size_t Segment::getDeletedCount() const
{
    return meta.deleted_count;
}

const std::string & Segment::getStoragePath() const
{
    return meta.storage_path;
}

common::IndexType Segment::getIndexType() const
{
    return meta.index_type;
}

void Segment::setMeta(const SegmentMeta & meta)
{
    this->meta = meta;
}

void Segment::setStatus(SegmentStatus status)
{
    this->meta.status = status;
}

} // namespace vectordb::core
