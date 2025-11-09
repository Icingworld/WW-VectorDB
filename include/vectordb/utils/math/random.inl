#pragma once

#include "vectordb/utils/math/random.h"

namespace vectordb::utils
{

template <typename RandomEngine>
template <typename... Args>
RandomGenerator<RandomEngine>::RandomGenerator(Args &&... args)
    : engine(std::forward<Args>(args)...)
{
}

template <typename RandomEngine>
template <typename... Args>
void RandomGenerator<RandomEngine>::seed(Args &&... args)
{
    engine.seed(std::forward<Args>(args)...);
}

template <typename RandomEngine>
template <
    typename DistributionType,
    typename... Args
> typename DistributionType::result_type RandomGenerator<RandomEngine>::distribution(Args &&... args)
{
    DistributionType dist(std::forward<Args>(args)...);
    return dist(engine);
}

template <typename RandomEngine>
bool RandomGenerator<RandomEngine>::bernoulliDistribution(double p)
{
    return distribution<std::bernoulli_distribution>(p);
}

template <typename RandomEngine>
template <typename NumberType>
NumberType RandomGenerator<RandomEngine>::normalDistribution(NumberType mean, NumberType stddev)
{
    return distribution<std::normal_distribution<NumberType>>(mean, stddev);
}

template <typename RandomEngine>
template <typename NumberType>
NumberType RandomGenerator<RandomEngine>::uniformDistribution(NumberType min, NumberType max)
{
    return distribution<UniformDistribution_t<NumberType>>(min, max);
}

template <
    typename DistributionType,
    typename RandomEngine
>
template <
    typename SeedType,
    typename... Args
> DistributionRandomGenerator<DistributionType, RandomEngine>::DistributionRandomGenerator(SeedType && seed, Args &&... args)
    : dist(std::forward<Args>(args)...)
    , engine(seed)
{
}

template <
    typename DistributionType,
    typename RandomEngine
>
template <typename... Args>
void DistributionRandomGenerator<DistributionType, RandomEngine>::seed(Args &&... args)
{
    engine.seed(std::forward<Args>(args)...);
}

template <
    typename DistributionType,
    typename RandomEngine
>
typename DistributionType::result_type DistributionRandomGenerator<DistributionType, RandomEngine>::generate()
{
    return dist(engine);
}

} // namespace vectordb::utils
