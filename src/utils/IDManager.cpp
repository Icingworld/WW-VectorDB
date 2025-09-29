#include "IDManager.h"

#include <fstream>

namespace WW
{

IDManager::IDManager()
    : IDManager("global_id.bin")
{
}

IDManager::IDManager(const std::string & path)
    : path_(path)
    , last_id_(0)
{
    Load();
}

IDManager & IDManager::GetInstance()
{
    static IDManager manager;
    return manager;
}

IDManager & IDManager::GetInstance(const std::string & path)
{
    static IDManager manager(path);
    return manager;
}

IDManager::~IDManager()
{
    Persist();
}

int64_t IDManager::Current()
{
    return last_id_.load();
}

int64_t IDManager::Next()
{
    return last_id_.fetch_add(1, std::memory_order_relaxed) + 1;
}

void IDManager::Load()
{
    std::ifstream fin(path_, std::ios::binary);
    if (!fin.is_open()) {
        last_id_ = 0;
        return;
    }

    int64_t id = 0;
    fin.read(reinterpret_cast<char *>(&id), sizeof(id));
    if (fin.gcount() == sizeof(id)) {
        last_id_ = id;
    } else {
        last_id_ = 0;
    }
}

void IDManager::Persist()
{
    std::ofstream fout(path_, std::ios::binary | std::ios::trunc);
    int64_t id_to_save = last_id_.load();
    fout.write(reinterpret_cast<const char *>(&id_to_save), sizeof(id_to_save));
}

} // namespace WW