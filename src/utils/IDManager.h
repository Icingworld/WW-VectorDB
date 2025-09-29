#pragma once

#include <string>
#include <mutex>
#include <atomic>

namespace WW
{

class IDManager
{
public:
    ~IDManager();

    static IDManager & GetInstance();

    static IDManager & GetInstance(const std::string & path);

    int64_t Current();

    int64_t Next();

private:
    IDManager();

    explicit IDManager(const std::string & path);

    IDManager(const IDManager &) = delete;

    IDManager & operator=(const IDManager &) = delete;

    void Load();

    void Persist();

private:
    std::string path_;
    std::atomic<int64_t> last_id_;
};

} // namespace WW
