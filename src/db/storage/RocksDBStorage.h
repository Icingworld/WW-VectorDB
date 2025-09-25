#pragma once

#include <rocksdb/db.h>
#include <rocksdb/options.h>
#include <rocksdb/write_batch.h>
#include <rocksdb/slice.h>

#include "Storage.h"

namespace WW
{

/**
 * @brief KV Storage Deleter
 */
struct RocksDBDeleter
{
    void operator()(rocksdb::DB * db) const
    {
        delete db;
    }
};

/**
 * @brief KV Storage Implementation
 * @details Based on RocksDB
 */
class RocksStorage : public IStorage
{
public:
    RocksStorage();

    ~RocksStorage() override;

    [[nodiscard]] bool Open(const std::string & path) override;

    void Close() override;

    [[nodiscard]] bool Put(const std::string & key, const std::string & value) override;

    [[nodiscard]] std::optional<std::string> Get(const std::string & key) const override;

    [[nodiscard]] bool Delete(const std::string & key) override;

    [[nodiscard]] bool BatchPut(const std::vector<std::pair<std::string, std::string>> & kvs) override;

    [[nodiscard]] std::vector<std::optional<std::string>> BatchGet(const std::vector<std::string> & keys) const override;

private:
    std::unique_ptr<rocksdb::DB, RocksDBDeleter> db_;
    rocksdb::Options options_;
};

} // namespace WW
