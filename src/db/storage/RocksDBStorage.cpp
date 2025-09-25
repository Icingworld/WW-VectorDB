#include "RocksDBStorage.h"

namespace WW
{

RocksStorage::RocksStorage()
{
    // set options
    options_.create_if_missing = true;
}

RocksStorage::~RocksStorage()
{
    Close();
}

bool RocksStorage::Open(const std::string & path)
{
    // open db
    rocksdb::DB * raw_ptr = nullptr;
    rocksdb::Status status = rocksdb::DB::Open(options_, path, &raw_ptr);
    if (!status.ok()) {
        return false;
    }
    db_.reset(raw_ptr);
    return true;
}

void RocksStorage::Close()
{
    db_.reset();
}

bool RocksStorage::Put(const std::string & key, const std::string & value)
{
    if (db_ == nullptr) {
        return false;
    }

    rocksdb::Status status = db_->Put(rocksdb::WriteOptions(), key, value);
    return status.ok();
}

std::optional<std::string> RocksStorage::Get(const std::string & key) const
{
    if (db_ == nullptr) {
        return std::nullopt;
    }

    std::string value;
    rocksdb::Status status = db_->Get(rocksdb::ReadOptions(), key, &value);
    if (!status.ok()) {
        return std::nullopt;
    }

    return value;
}

bool RocksStorage::Delete(const std::string & key)
{
    if (db_ == nullptr) {
        return false;
    }

    rocksdb::Status status = db_->Delete(rocksdb::WriteOptions(), key);
    return status.ok();
}

bool RocksStorage::BatchPut(const std::vector<std::pair<std::string, std::string>> & kvs)
{
    if (db_ == nullptr) {
        return false;
    }

    rocksdb::WriteBatch batch;
    for (const auto & [key, value] : kvs) {
        batch.Put(key, value);
    }

    rocksdb::Status status = db_->Write(rocksdb::WriteOptions(), &batch);
    return status.ok();
}

std::vector<std::optional<std::string>> RocksStorage::BatchGet(const std::vector<std::string> & keys) const
{
    if (db_ == nullptr) {
        return {};
    }

    // convert keys to Slices
    std::vector<rocksdb::Slice> slices;
    slices.reserve(keys.size());
    for (auto & k : keys) {
        slices.emplace_back(k);
    }

    // prepare results
    std::vector<std::string> values(keys.size());
    std::vector<std::optional<std::string>> results(keys.size());

    // MultiGet
    rocksdb::ReadOptions read_options;
    auto statuses = db_->MultiGet(read_options, slices, &values);

    // convert optional
    for (size_t i = 0; i < statuses.size(); i++) {
        if (statuses[i].ok()) {
            results[i] = std::move(values[i]);
        } else {
            results[i] = std::nullopt;
        }
    }
    return results;
}


} // namespace WW
