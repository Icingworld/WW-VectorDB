#pragma once

#include <string>
#include <vector>
#include <memory>
#include <optional>

#include "Storage.h"
#include "Index.h"

namespace WW
{

/**
 * @brief VectorDB Base
 */
class IVectorDB
{
public:
    virtual ~IVectorDB() = default;

    /**
     * @brief Open database
     * @param path Path to database
     * @param dim Vector dimension
     * @return True on success
     */
    [[nodiscard]] virtual bool Open(const std::string & path, int dim) = 0;

    /**
     * @brief Close database
     */
    virtual void Close() = 0;

    /**
     * @brief Upsert vector
     * @param id ID
     * @param vector Vector
     * @return True on success
     */
    [[nodiscard]] virtual bool Upsert(const std::string & id, const std::vector<float> & vector) = 0;

    /**
     * @brief Delete vector
     * @param id ID
     * @return True on success
     */
    [[nodiscard]] virtual bool Delete(const std::string & id) = 0;

    /**
     * @brief Get vector
     * @param id ID
     * @return Optional vector
     */
    [[nodiscard]] virtual std::optional<std::vector<float>> Get(const std::string & id) const = 0;

    /**
     * @brief Search vectors
     * @param query Query vector
     * @param top_k Top K
     * @return Vectors
     */
    [[nodiscard]] virtual std::vector<SearchResult> Search(const std::vector<float> & query, int top_k) const = 0;

    /**
     * @brief Upsert multiple vectors
     * @param items Items
     * @return True on success
     */
    [[nodiscard]] virtual bool BatchUpsert(const std::vector<std::pair<std::string, std::vector<float>>> & items) = 0;

    /**
     * @brief Get multiple vectors
     * @param ids IDs
     * @param vectors Vectors
     * @return Vectors
     */
    [[nodiscard]] virtual std::vector<std::optional<std::vector<float>>> BatchGet(const std::vector<std::string> & ids) const = 0;
};

/**
 * @brief VectorDB Implementation
 * @details Based on RocksDB
 */
class RocksVectorDB : public IVectorDB
{
public:
    RocksVectorDB() = default;

    ~RocksVectorDB() override = default;

    [[nodiscard]] bool Open(const std::string & path, int dim) override;
    
    void Close() override;
    
    [[nodiscard]] bool Upsert(const std::string & id, const std::vector<float> & vector) override;
    
    [[nodiscard]] bool Delete(const std::string & id) override;
    
    [[nodiscard]] std::optional<std::vector<float>> Get(const std::string & id) const override;
    
    [[nodiscard]] std::vector<SearchResult> Search(const std::vector<float> & query, int top_k) const override;
    
    [[nodiscard]] bool BatchUpsert(const std::vector<std::pair<std::string, std::vector<float>>> & items) override;
    
    [[nodiscard]] std::vector<std::optional<std::vector<float>>> BatchGet(const std::vector<std::string> & ids) const override;

private:
    int dimension_;
    std::unique_ptr<RocksStorage> storage_;
    std::unique_ptr<BruteForceIndex> index_;
};

} // namespace WW
