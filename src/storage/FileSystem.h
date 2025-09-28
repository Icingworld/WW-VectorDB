#pragma once

#include <string>
#include <vector>

namespace WW
{

class IFileSystem
{
public:
    virtual ~IFileSystem() = default;

    virtual bool Exists(const std::string & path) const = 0;

    virtual void WriteFile(const std::string & path, const std::vector<char> & data) = 0;

    virtual std::vector<char> ReadFile(const std::string & path) = 0;

    virtual void Remove(const std::string & path) = 0;

    virtual void Mkdir(const std::string & path) = 0;
};
    
} // namespace WW
