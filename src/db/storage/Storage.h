#pragma once

#include <string>
#include <vector>
#include <memory>
#include <optional>

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

} // namespace WW
