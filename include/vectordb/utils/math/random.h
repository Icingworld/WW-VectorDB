#pragma once

#include <random>
#include <type_traits>

namespace vectordb::utils
{

/**
 * @brief 均匀分布
 * @tparam NumberType 数值类型，仅支持 `int` 和 `real`
 */
template <
    typename NumberType,
    typename Enable = void
>
class UniformDistribution;

template <typename NumberType>
class UniformDistribution<NumberType, std::enable_if_t<std::is_integral_v<NumberType>>>
{
public:
    using type = std::uniform_int_distribution<NumberType>;
};

template <typename NumberType>
class UniformDistribution<NumberType, std::enable_if_t<std::is_floating_point_v<NumberType>>>
{
public:
    using type = std::uniform_real_distribution<NumberType>;
};

template <typename NumberType>
using UniformDistribution_t = typename UniformDistribution<NumberType>::type;

/**
* @brief 随机数发生器
* @tparam RandomEngine 随机数引擎，默认为 `std::default_random_engine`
*/
template <typename RandomEngine = std::default_random_engine>
class RandomGenerator
{
public:
    /**
    * @brief 随机数发生器
    * @param args 随机数引擎参数
    */
    template <typename... Args>
    explicit RandomGenerator(Args &&... args);

    ~RandomGenerator() = default;

    /**
    * @brief 设置随机数种子
    * @param args 随机数种子参数
    */
    template <typename... Args>
    void seed(Args&&... args);

    /**
    * @brief 生成随机数
    * @tparam DistributionType 随机数分布类型
    * @param args 随机数分布参数
    */
    template <
        typename DistributionType,
        typename... Args
    >
    typename DistributionType::result_type distribution(Args &&... args);

    /**
    * @brief 获取指定概率下伯努利分布的随机数
    * @param p 概率
    * @return 伯努利分布
    */
    bool bernoulliDistribution(double p);

    /**
    * @brief 获取指定均值和标准差下正态分布的随机数
    * @param mean 均值
    * @param stddev 标准差
    * @return 正态分布
    */
    template <typename NumberType>
    NumberType normalDistribution(NumberType mean, NumberType stddev);

    /**
    * @brief 获取指定范围下均匀分布的随机数
    * @tparam NumberType 数值类型
    * @param min 最小值
    * @param max 最大值
    * @return 均匀分布
    */
    template <typename NumberType>
    NumberType uniformDistribution(NumberType min, NumberType max);

private:
    RandomEngine engine;
};

template <
    typename DistributionType,
    typename RandomEngine = std::default_random_engine
>
class DistributionRandomGenerator
{
public:
    template <
        typename SeedType,
        typename... Args
    >
    DistributionRandomGenerator(SeedType && seed, Args &&... args);

    ~DistributionRandomGenerator() = default;

    /**
    * @brief 设置随机数种子
    * @param args 随机数种子参数
    */
    template <typename... Args>
    void seed(Args &&... args);

    /**
    * @brief 生成随机数
    */
    typename DistributionType::result_type generate();

private:
    DistributionType dist;
    RandomEngine engine;
};

} // namespace vectordb::utils

#include "vectordb/utils/math/random.inl"
