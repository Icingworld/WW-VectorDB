#include "VectorDB.h"

namespace WW {

bool RocksVectorDB::Open(const std::string & path, int dim)
{
    if (dim <= 0) {
        return false;
    }

    storage_ = std::make_unique<RocksStorage>();
    if (!storage_->Open(path)) {
        return false;
    }

    index_ = std::make_unique<BruteForceIndex>(dim);
    dimension_ = dim;

    return true;
}

void RocksVectorDB::Close()
{
    if (storage_) {
        storage_->Close();
    }
    index_.reset();
    storage_.reset();
    dimension_ = 0;
}

bool RocksVectorDB::Upsert(const std::string & id, const std::vector<float> & vector)
{
    if (vector.size() != static_cast<size_t>(dimension_)) {
        return false;
    }

    std::string blob(reinterpret_cast<const char*>(vector.data()), vector.size() * sizeof(float));
    
    if (!storage_->Put(id, blob)) {
        return false;
    }
    return index_->Add(id, vector);
}

bool RocksVectorDB::Delete(const std::string & id)
{
    if (!storage_->Delete(id)) {
        return false;
    }
    return index_->Remove(id);
}

std::optional<std::vector<float>> RocksVectorDB::Get(const std::string & id) const
{
    auto opt_blob = storage_->Get(id);
    if (!opt_blob.has_value()) {
        return std::nullopt;
    }

    const std::string & blob = *opt_blob;
    if (blob.size() % sizeof(float) != 0) {
        return std::nullopt;
    }

    size_t n = blob.size() / sizeof(float);
    std::vector<float> vec(n);
    std::memcpy(vec.data(), blob.data(), blob.size());
    return vec;
}

std::vector<SearchResult> RocksVectorDB::Search(const std::vector<float> & query, int top_k) const
{
    if (query.size() != static_cast<size_t>(dimension_)) {
        return {};
    }
    
    return index_->Search(query, top_k);
}

bool RocksVectorDB::BatchUpsert(const std::vector<std::pair<std::string, std::vector<float>>> & items)
{

    // build write batch for storage
    std::vector<std::pair<std::string, std::string>> batch;
    batch.reserve(items.size());

    for (const auto & [id, vec] : items) {
        if (vec.size() != static_cast<size_t>(dimension_)) {
            return false;
        }
        std::string blob(reinterpret_cast<const char*>(vec.data()), vec.size() * sizeof(float));
        batch.emplace_back(id, blob);
    }

    if (!storage_->BatchPut(batch)) {
        return false;
    }

    for (const auto & [id, vec] : items) {
        if (!index_->Add(id, vec)) {
            return false;
        }
    }

    return true;
}

std::vector<std::optional<std::vector<float>>> RocksVectorDB::BatchGet(const std::vector<std::string> & ids) const
{
    
    auto blobs = storage_->BatchGet(ids);

    std::vector<std::optional<std::vector<float>>> results;
    results.reserve(blobs.size());

    for (const auto & opt_blob : blobs) {
        if (!opt_blob.has_value()) {
            results.emplace_back(std::nullopt);
            continue;
        }

        const std::string & blob = *opt_blob;
        if (blob.size() % sizeof(float) != 0) {
            results.emplace_back(std::nullopt);
            continue;
        }

        size_t n = blob.size() / sizeof(float);
        std::vector<float> vec(n);
        std::memcpy(vec.data(), blob.data(), blob.size());
        results.emplace_back(std::move(vec));
    }

    return results;
}

} // namespace WW