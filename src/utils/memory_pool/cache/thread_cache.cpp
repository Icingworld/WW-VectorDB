#include "vectordb/utils/memory_pool/cache/thread_cache.h"

#include <new>

#include "vectordb/utils/memory_pool/common/size.h"
#include "vectordb/utils/memory_pool/cache/central_cache.h"

namespace vectordb::utils
{

ThreadCache::ThreadCache()
    : free_lists()
{
}

ThreadCache::~ThreadCache()
{
    // 归还所有内存块
    for (size_type i = 0; i < free_lists.size(); ++i) {
        if (!free_lists[i].empty()) {
            returnToCentralCache(i, free_lists[i].size());
        }
    }
}

ThreadCache & ThreadCache::getThreadCache()
{
    // 线程局部存储的单例
    static thread_local ThreadCache thread_cache;
    return thread_cache;
}

void * ThreadCache::allocate(size_type size) noexcept
{
    if (size == 0) {
        return nullptr;
    }

    if (size > MAX_MEMORY_SIZE) {
        // 超出管理范围，直接从堆获取
        return ::operator new(size, std::nothrow);
    }

    // 获取对齐后的大小
    size_type round_size = Size::roundUp(size);
    // 找到所在的索引
    size_type index = Size::sizeToIndex(round_size);

    if (free_lists[index].empty()) {
        // 没有这种内存块，需要申请
        fetchFromCentralCache(round_size);
    }

    // 有这种内存块，取一个出来
    FreeObject * object = free_lists[index].front();
    free_lists[index].popFront();
    return reinterpret_cast<void *>(object);
}

void ThreadCache::deallocate(void * ptr, size_type size) noexcept
{
    if (size == 0) {
        return;
    }

    if (size > MAX_MEMORY_SIZE) {
        // 超出管理范围，直接释放
        ::operator delete(ptr, std::nothrow);
        return;
    }

    // 获取对齐后的大小
    size_type round_size = Size::roundUp(size);
    // 找到所在的索引
    size_type index = Size::sizeToIndex(round_size);
    // 把内存插入自由表
    FreeObject * object = reinterpret_cast<FreeObject *>(ptr);
    free_lists[index].pushFront(object);

    // 检查是否需要归还给中心缓存
    if (shouldReturn(index)) {
        returnToCentralCache(index, free_lists[index].maxSize());
    }
}

bool ThreadCache::shouldReturn(size_type index) const noexcept
{
    if (free_lists[index].size() >= free_lists[index].maxSize() * 2) {
        // 超过一次申请的最大数量的两倍，归还一半
        return true;
    }

    return false;
}

void ThreadCache::fetchFromCentralCache(size_type size) noexcept
{
    // 每次申请按照最大数量申请，并且提升最大数量
    size_type index = Size::sizeToIndex(size);
    size_type count = free_lists[index].maxSize();
    if (count > MAX_BLOCK_NUM) {
        count = MAX_BLOCK_NUM;
    }

    FreeObject * object = CentralCache::getCentralCache().fetchRange(size, count);
    FreeObject * current = object;

    while (current != nullptr) {
        FreeObject * next = current->next();
        free_lists[index].pushFront(current);
        current = next;
    }

    // 提升最大数量
    free_lists[index].setMaxSize(count + 1);
}

void ThreadCache::returnToCentralCache(size_type index, size_type nums) noexcept
{
    // 取出 nums 个内存块组成链表
    FreeObject * head = nullptr;

    for (size_type i = 0; i < nums; ++i) {
        FreeObject * object = free_lists[index].front();
        free_lists[index].popFront();
        object->setNext(head);
        head = object;
    }

    CentralCache::getCentralCache().returnRange(Size::indexToSize(index), head);
}

} // namespace vectordb::utils
