#pragma once

#include <string>
#include <vector>
#include <memory>
#include <optional>

#include <rocksdb/db.h>
#include <rocksdb/options.h>
#include <rocksdb/write_batch.h>
#include <rocksdb/slice.h>

namespace WW
{

/**
 * @brief KV Storage Base
 */
class IStorage
{
public:
    virtual ~IStorage() = default;

    /**
     * @brief Open database
     * @param path Path to database
     * @return True on success
     */
    [[nodiscard]] virtual bool Open(const std::string & path) = 0;

    /**
     * @brief Close database
     */
    virtual void Close() = 0;

    /**
     * @brief Put value to database
     * @param key Key
     * @param value Value
     * @return True on success
     */
    [[nodiscard]] virtual bool Put(const std::string & key, const std::string & value) = 0;

    /**
     * @brief Get value from database
     * @param key Key
     * @param value Value
     * @return Optional value
     */
    [[nodiscard]] virtual std::optional<std::string> Get(const std::string & key) const = 0;

    /**
     * @brief Delete value from database
     * @param key Key
     * @return True on success
     */
    [[nodiscard]] virtual bool Delete(const std::string & key) = 0;

    /**
     * @brief Put multiple values to database
     * @param kvs Key-value pairs
     * @return True on success
     */
    [[nodiscard]] virtual bool BatchPut(const std::vector<std::pair<std::string, std::string>> & kvs) = 0;

    /**
     * @brief Get multiple values from database
     * @param keys Keys
     * @param values Values
     * @return Optional values
     */
    [[nodiscard]] virtual std::vector<std::optional<std::string>> BatchGet(const std::vector<std::string> & keys) const = 0;
};

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
