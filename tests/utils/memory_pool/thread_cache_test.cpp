#include <atomic>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <thread>
#include <vector>

#include "vectordb/utils/memory_pool/cache/thread_cache.h"

using vectordb::utils::ThreadCache;

namespace
{

constexpr std::size_t kAllocSize = 64;
constexpr std::size_t kIterationsPerThread = 512;
constexpr std::size_t kThreadCount = 4;

void worker(std::atomic<bool> & failed)
{
    auto & cache = ThreadCache::getThreadCache();
    std::vector<void *> objects;
    objects.reserve(kIterationsPerThread);

    for (std::size_t i = 0; i < kIterationsPerThread; ++i) {
        void * ptr = cache.allocate(kAllocSize);
        if (ptr == nullptr) {
            failed.store(true, std::memory_order_relaxed);
            return;
        }

        std::memset(ptr, static_cast<int>(i & 0xFF), kAllocSize);
        objects.emplace_back(ptr);
    }

    for (std::size_t i = 0; i < objects.size(); ++i) {
        std::uint8_t * data = static_cast<std::uint8_t *>(objects[i]);
        if (data[0] != static_cast<std::uint8_t>(i & 0xFF)) {
            failed.store(true, std::memory_order_relaxed);
            return;
        }
        cache.deallocate(objects[i], kAllocSize);
    }
}

} // namespace

int main()
{
    std::atomic<bool> failed { false };
    std::vector<std::thread> threads;
    threads.reserve(kThreadCount);

    for (std::size_t i = 0; i < kThreadCount; ++i) {
        threads.emplace_back(worker, std::ref(failed));
    }

    for (auto & t : threads) {
        t.join();
    }

    if (failed.load(std::memory_order_relaxed)) {
        std::cerr << "Thread cache concurrency test failed" << std::endl;
        return 1;
    }

    auto & main_cache = ThreadCache::getThreadCache();
    void * ptr = main_cache.allocate(kAllocSize);
    if (ptr == nullptr) {
        std::cerr << "Main thread allocation failed" << std::endl;
        return 1;
    }
    std::memset(ptr, 0xAB, kAllocSize);
    main_cache.deallocate(ptr, kAllocSize);

    return 0;
}

