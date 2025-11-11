#pragma once

#include <string>
#include <vector>
#include <optional>
#include <utility>
#include <unordered_map>

#include "vectordb/common/type.h"
#include "vectordb/common/json.h"

namespace vectordb::core
{

/**
 * @brief 段状态
 */
enum class SegmentStatus : std::uint8_t
{
    growing = 0,        // 增长中
    sealed = 1,         // 密封
    deleted = 2,        // 删除
};

/**
 * @brief 段元数据
 */
class SegmentMeta
{
public:
    SegmentMeta() = default;

    ~SegmentMeta() = default;

public:
    /**
     * @brief 转换为 JSON
     * @return JSON
     */
    common::Json toJson() const;

    /**
     * @brief 从 JSON 转换
     * @param json JSON
     */
    void FromJson(const common::Json & json);

public:
    std::string segment_id;                 // segment 唯一标识
    std::string collection_id;              // 所属集合唯一标识
    SegmentStatus status;                   // 段状态
    std::size_t row_count;                  // 有效行数
    std::size_t deleted_count;              // 删除行数
    std::string storage_path;               // 存储路径
    common::IndexType index_type;           // 索引类型
};

/**
 * @brief 段
 */
class Segment
{
public:
    explicit Segment(const SegmentMeta & meta);

    ~Segment() = default;

public:
    /**
     * @brief 加载段
     */
    void loadSegment();

    /**
     * @brief 保存段
     */
    void saveSegment();

    /**
     * @brief 密封段
     */
    void sealSegment();

    /**
     * @brief 删除段
     */
    void removeSegment();

    /**
     * @brief 插入向量
     * @param vector 向量
     */
    void insertVector(const std::string & vector_id, const std::vector<float> & vector);

    /**
     * @brief 删除向量
     * @param vector 向量
     */
    void deleteVector(const std::string & vector_id);

    /**
     * @brief 获取向量
     * @param vector_id 向量ID
     * @return 向量
     */
    std::optional<std::reference_wrapper<const std::vector<float>>> getVector(const std::string & vector_id) const;

    /**
     * @brief 获取段元数据
     * @return 段元数据
     */
    const SegmentMeta & getMeta() const;

    /**
     * @brief 获取段唯一标识
     * @return 段唯一标识
     */
    const std::string & getSegmentId() const;

    /**
     * @brief 获取所属集合唯一标识
     * @return 所属集合唯一标识
     */
    const std::string & getCollectionId() const;

    /**
     * @brief 获取段状态
     * @return 段状态
     */
    SegmentStatus getStatus() const;

    /**
     * @brief 获取有效行数
     * @return 有效行数
     */
    std::size_t getRowCount() const;

    /**
     * @brief 获取删除行数
     * @return 删除行数
     */
    std::size_t getDeletedCount() const;

    /**
     * @brief 获取存储路径
     * @return 存储路径
     */
    const std::string & getStoragePath() const;

    /**
     * @brief 获取索引类型
     * @return 索引类型
     */
    common::IndexType getIndexType() const;

    /**
     * @brief 设置段元数据
     * @param meta 段元数据
     */
    void setMeta(const SegmentMeta & meta);

    /**
     * @brief 设置段状态
     * @param status 段状态
     */
    void setStatus(SegmentStatus status);

private:
    SegmentMeta meta;               // 段元数据
    std::vector<
        std::pair<std::string, std::vector<float>>
    > vectors;                      // 向量数据
    std::unordered_map<std::string, std::size_t> vector_index;  // 向量索引 id -> 向量偏移
};

} // namespace vectordb::core
