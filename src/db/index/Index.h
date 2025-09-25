#pragma once

#include <string>
#include <vector>
#include <memory>
#include <optional>

namespace WW
{

/**
 * @brief Search result
 * @details ID and score. vector<float> is not supported
 */
struct SearchResult
{
    std::string id;
    float score;
};

/**
 * @brief Index Base
 */
class IIndex
{
public:
    virtual ~IIndex() = default;

    /**
     * @brief Add vector to index
     * @param id ID
     * @param vector Vector
     * @return True on success
     */
    [[nodiscard]] virtual bool Add(const std::string & id, const std::vector<float> & vector) = 0;

    /**
     * @brief Remove vector from index
     * @param id ID
     * @return True on success
     */
    [[nodiscard]] virtual bool Remove(const std::string & id) = 0;

    /**
     * @brief Get vector from index
     * @param id ID
     * @return Optional vector
     */
    [[nodiscard]] virtual std::optional<std::vector<float>> Get(const std::string & id) const = 0;

    /**
     * @brief Search vector in index
     * @param vector Vector
     * @param top_k Top K
     * @return Search results
     */
    [[nodiscard]] virtual std::vector<SearchResult> Search(const std::vector<float> & query, int top_k) const = 0;

    /**
     * @brief Batch add vectors to index
     * @param items Items
     * @return True on success
     */
    [[nodiscard]] virtual bool BatchAdd(const std::vector<std::pair<std::string, std::vector<float>>> & items) = 0;

    /**
     * @brief Batch remove vectors from index
     * @param ids IDs
     * @return True on success
     */
    [[nodiscard]] virtual bool BatchRemove(const std::vector<std::string> & ids) = 0;

    /**
     * @brief Batch get vectors from index
     * @param ids IDs
     * @return Vectors
     */
    [[nodiscard]] virtual std::vector<std::optional<std::vector<float>>> BatchGet(const std::vector<std::string> & ids) const = 0;

    /**
     * @brief Get size of index
     * @return Size
     */
    [[nodiscard]] virtual std::size_t Size() const = 0;

    /**
     * @brief Get dimension of index
     * @return Dimension
     */
    [[nodiscard]] virtual int Dimension() const = 0;
};

} // namespace WW
