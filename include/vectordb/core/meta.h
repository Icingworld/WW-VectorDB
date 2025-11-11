#pragma once

#include "vectordb/common/json.h"

namespace vectordb::core
{

/**
 * @brief 元数据基类
 */
class MetaBase
{
public:
    explicit MetaBase() = default;

    virtual ~MetaBase() = default;

public:
    /**
     * @brief 从 JSON 转换
     * @param json JSON
     */
    virtual void fromJson(const common::Json & json) = 0;

    /**
     * @brief 转换为 JSON
     * @return JSON
     */
    virtual common::Json toJson() const = 0;
};

/**
 * @brief 保存元数据到磁盘
 * @param meta 元数据
 * @param path 元数据路径
 */
template <typename MetaType>
void SaveMeta(const MetaType & meta, const std::string & path);

/**
* @brief 从磁盘加载元数据
* @param meta 元数据
* @param path 元数据路径
*/
template <typename MetaType>
void LoadMeta(MetaType & meta,const std::string & path);

} // namespace vectordb::core

#include "vectordb/core/meta.inl"
