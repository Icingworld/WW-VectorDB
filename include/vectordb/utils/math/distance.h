#pragma once

#include <cstddef>
#include <type_traits>

namespace vectordb::utils
{

/**
 * @brief 距离计算类
 */
class Distance
{
public:
    /**
    * @brief 计算两个向量之间的 L2 距离平方
    * @tparam T 浮点类型
    * @param vec1 指向向量 1 的指针
    * @param vec2 指向向量 2 的指针
    * @param size 向量长度
    * @return L2 距离平方
    */
    template <typename T>
    static T l2DistanceSquared(const T * vec1, const T * vec2, std::size_t size);

    /**
    * @brief 计算两个容器之间的 L2 距离平方
    * @tparam Container 容器类型
    * @tparam Value 容器值类型
    * @tparam Enable 启用条件
    * @param vec1 容器 1
    * @param vec2 容器 2
    * @return L2 距离平方
    */
    template <
        typename Container,
        typename Value = typename Container::value_type,
        typename Enable = std::enable_if_t<std::is_floating_point_v<Value>>
    >
    static Value l2DistanceSquared(const Container & vec1, const Container & vec2);

    /**
    * @brief 计算两个向量之间的 L2 距离
    * @tparam T 浮点类型
    * @param vec1 指向向量 1 的指针
    * @param vec2 指向向量 2 的指针
    * @param size 向量长度
    * @return L2 距离
    */
    template <typename T>
    static T l2Distance(const T * vec1, const T * vec2, std::size_t size);

    /**
    * @brief 计算两个容器之间的 L2 距离
    */
    template <
        typename Container,
        typename Value = typename Container::value_type,
        typename Enable = std::enable_if_t<std::is_floating_point_v<Value>>
    >
    static Value l2Distance(const Container & vec1, const Container & vec2);

    /**
    * @brief 计算两个向量之间的余弦相似度
    * @tparam T 浮点类型
    * @param vec1 指向向量 1 的指针
    * @param vec2 指向向量 2 的指针
    * @param size 向量长度
    * @return 余弦相似度
    */
    template <typename T>
    static T cosineSimilarity(const T * vec1, const T * vec2, std::size_t size);

    /**
    * @brief 计算两个容器之间的余弦相似度
    */
    template <
        typename Container,
        typename Value = typename Container::value_type,
        typename Enable = std::enable_if_t<std::is_floating_point_v<Value>>
    >
    static Value cosineSimilarity(const Container & vec1, const Container & vec2);

private:
    /**
    * @brief 尺寸校验
    */
    static void ensureSameSize(std::size_t left, std::size_t right);

    template <typename T>
    static void ensurePointerValid(const T * ptr, std::size_t size);
};

} // namespace vectordb::utils

#include "vectordb/utils/math/distance.inl"
