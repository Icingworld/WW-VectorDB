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
    for (size_type i = 0; i < free_lists.size(); ++i) {
        if (!free_lists[i].empty()) {
            returnToCentralCache(i, free_lists[i].size());
        }
    }
}

ThreadCache & ThreadCache::getThreadCache()
{
    static thread_local ThreadCache thread_cache;
    return thread_cache;
}

void * ThreadCache::allocate(size_type size) noexcept
{
    if (size == 0) {
        return nullptr;
    }

    if (size > MAX_MEMORY_SIZE) {
        return ::operator new(size, std::nothrow);
    }

    size_type round_size = Size::roundUp(size);
    size_type index = Size::sizeToIndex(round_size);

    if (free_lists[index].empty()) {
        fetchFromCentralCache(round_size);
    }

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
        ::operator delete(ptr, std::nothrow);
        return;
    }

    size_type round_size = Size::roundUp(size);
    size_type index = Size::sizeToIndex(round_size);
    FreeObject * object = reinterpret_cast<FreeObject *>(ptr);
    free_lists[index].pushFront(object);

    if (shouldReturn(index)) {
        returnToCentralCache(index, free_lists[index].maxSize());
    }
}

bool ThreadCache::shouldReturn(size_type index) const noexcept
{
    if (free_lists[index].size() >= free_lists[index].maxSize() * 2) {
        return true;
    }

    return false;
}

void ThreadCache::fetchFromCentralCache(size_type size) noexcept
{
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

    free_lists[index].setMaxSize(count + 1);
}

void ThreadCache::returnToCentralCache(size_type index, size_type nums) noexcept
{
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
