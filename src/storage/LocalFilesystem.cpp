#include "LocalFilesystem.h"

#include <filesystem>
#include <fstream>

namespace WW
{

bool LocalFileSystem::Exists(const std::string & path) const
{
    return std::filesystem::exists(path);
}

void LocalFileSystem::WriteFile(const std::string & path, const std::vector<char> & data)
{
    std::ofstream ofs(path, std::ios::binary);
    ofs.write(data.data(), data.size());
}

std::vector<char> LocalFileSystem::ReadFile(const std::string & path)
{
    std::ifstream ifs(path, std::ios::binary);
    return std::vector<char>((std::istreambuf_iterator<char>(ifs)), 
            std::istreambuf_iterator<char>());
}

void LocalFileSystem::Remove(const std::string & path)
{
    std::filesystem::remove(path);
}

void LocalFileSystem::Mkdir(const std::string & path)
{
    std::filesystem::create_directory(path);
}

} // namespace WW
