#pragma once

#include "vectordb/core/meta.h"

#include <fstream>

#include "vectordb/common/json.h"

namespace vectordb::core
{

template <typename MetaType>
void SaveMeta(const MetaType & meta, const std::string & path)
{
    std::ofstream ofs(path);
    if (!ofs.is_open()) {
        // TODO: 记录错误日志
        return;
    }

    ofs << meta.toJson().dump(4);
}

template <typename MetaType>
void LoadMeta(MetaType & meta, const std::string & path)
{
    std::ifstream ifs(path);
    if (!ifs.is_open()) {
        // TODO: 记录错误日志
        return;
    }

    common::Json j;
    ifs >> j;

    meta.fromJson(j);
}

} // namespace vectordb::core
