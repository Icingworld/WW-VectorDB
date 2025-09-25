#include "BruteForceIndex.h"

#include <cassert>
#include <algorithm>

#include <VectorUtils.h>

namespace WW
{

BruteForceIndex::BruteForceIndex(int dimension)
    : dimension_(dimension)
{
}

bool BruteForceIndex::Add(const std::string & id, const std::vector<float> & vector)
{
    assert(vector.size() == dimension_);
    return index_.emplace(id, vector).second;
}

bool BruteForceIndex::Remove(const std::string & id)
{
    return index_.erase(id) > 0;
}

std::optional<std::vector<float>> BruteForceIndex::Get(const std::string & id) const
{
    auto it = index_.find(id);
    if (it == index_.end()) {
        return std::nullopt;
    }
    return it->second;
}

std::vector<SearchResult> BruteForceIndex::Search(const std::vector<float> & query, int top_k) const
{
    std::vector<SearchResult> results;
    
    // check dimension
    if (static_cast<int>(query.size()) != dimension_) {
        return results;
    }

    // prepare result space
    results.reserve(std::min<std::size_t>(top_k, index_.size()));

    // compute distance
    for (const auto & [id, vector] : index_) {
        float distance = VectorUtils::L2DistanceSquared(query, vector);
        results.emplace_back(id, distance);
    }

    if (results.empty()) {
        return results;
    }

    // sort by distance
    if (static_cast<int>(results.size()) > top_k) {
        // partial sort to get top_k smallest
        std::partial_sort(results.begin(), results.begin() + top_k, results.end(), [](const SearchResult & a, const SearchResult & b) {
            return a.score < b.score;
        });

        results.resize(top_k);
    } else {
        // sort all
        std::sort(results.begin(), results.end(), [](const SearchResult & a, const SearchResult & b) {
            return a.score < b.score;
        });
    }

    return results;
}

bool BruteForceIndex::BatchAdd(const std::vector<std::pair<std::string, std::vector<float>>> & items)
{
    for (const auto & [id, vector] : items) {
        if (!Add(id, vector)) {
            return false;
        }
    }
    return true;
}

bool BruteForceIndex::BatchRemove(const std::vector<std::string> & ids)
{
    for (const auto & id : ids) {
        if (!Remove(id)) {
            return false;
        }
    }
    return true;
}

std::vector<std::optional<std::vector<float>>> BruteForceIndex::BatchGet(const std::vector<std::string> & ids) const
{
    std::vector<std::optional<std::vector<float>>> results;
    results.reserve(ids.size());
    for (const auto & id : ids) {
        results.emplace_back(Get(id));
    }
    return results;
}

std::size_t BruteForceIndex::Size() const
{
    return index_.size();
}

int BruteForceIndex::Dimension() const
{
    return dimension_;
}

} // namespace WW
