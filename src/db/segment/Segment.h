#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <cstddef>

#include "Index.h"
#include "LocalFilesystem.h"

namespace WW
{

/**
 * @brief meta data of segment
 */
struct SegmentMeta
{
    std::string id;                 // segment id
    std::string collection_id;      // belongs to which collection
    int dimension;                  // dimension of segment
    std::size_t row_count;          // current count
    bool is_sealed;                 // whether segment is sealed
    std::string path;               // path of storage
};

class Segment
{
public:
    explicit Segment(const SegmentMeta & meta);

    Segment();

    const std::string & Id() const;

    const std::string & CollectionId() const;

    int Dimension() const;
    
    std::size_t RowCount() const;

    bool IsSealed() const;

    const std::string & Path() const;

    void Seal();

    void Save();

    void Load();

    bool Upsert(const std::string & id, const std::vector<float> & vector);

    bool Delete(const std::string & id);

    std::optional<std::vector<float>> Get(const std::string & id) const;

    std::vector<SearchResult> Search(const std::vector<float> & query, int top_k) const;

private:
    SegmentMeta meta_;
    std::unordered_map<std::string, std::vector<float>> data_;
    std::unique_ptr<IIndex> index_;
    std::unique_ptr<IFileSystem> fs_;
};

} // namespace WW
