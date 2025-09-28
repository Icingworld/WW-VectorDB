#pragma once

#include "FileSystem.h"

namespace WW
{

class LocalFileSystem : public IFileSystem
{
public:
    bool Exists(const std::string & path) const override;

    void WriteFile(const std::string & path, const std::vector<char> & data) override;

    std::vector<char> ReadFile(const std::string & path) override;

    void Remove(const std::string & path) override;

    void Mkdir(const std::string & path) override;
};

} // namespace WW
