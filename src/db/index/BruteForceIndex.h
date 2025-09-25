#pragma once

#include <unordered_map>

#include "Index.h"

namespace WW
{

/**
 * @brief Brute Force Index
 */
class BruteForceIndex : public IIndex
{
public:
    explicit BruteForceIndex(int dimension);

    BruteForceIndex(const BruteForceIndex &) = delete;

    BruteForceIndex & operator=(const BruteForceIndex &) = delete;

    BruteForceIndex(BruteForceIndex &&) = default;

    BruteForceIndex & operator=(BruteForceIndex &&) = default;

    ~BruteForceIndex() override = default;

    [[nodiscard]] bool Add(const std::string & id, const std::vector<float> & vector) override;

    [[nodiscard]] bool Remove(const std::string & id) override;

    [[nodiscard]] std::optional<std::vector<float>> Get(const std::string & id) const override;

    [[nodiscard]] std::vector<SearchResult> Search(const std::vector<float> & query, int top_k) const override;

    [[nodiscard]] bool BatchAdd(const std::vector<std::pair<std::string, std::vector<float>>> & items) override;

    [[nodiscard]] bool BatchRemove(const std::vector<std::string> & ids) override;

    [[nodiscard]] std::vector<std::optional<std::vector<float>>> BatchGet(const std::vector<std::string> & ids) const override;

    [[nodiscard]] std::size_t Size() const override;

    [[nodiscard]] int Dimension() const override;

private:
    int dimension_;
    std::unordered_map<std::string, std::vector<float>> index_;
};

} // namespace WW
