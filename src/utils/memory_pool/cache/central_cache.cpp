#include "vectordb/utils/memory_pool/cache/central_cache.h"

#include "vectordb/utils/memory_pool/cache/page_cache.h"
#include "vectordb/utils/memory_pool/common/size.h"

namespace vectordb::utils
{

CentralCache::CentralCache()
    : spans()
{
}

CentralCache & CentralCache::getCentralCache()
{
    static CentralCache instance;
    return instance;
}

FreeObject * CentralCache::fetchRange(size_type size, size_type count)
{
    size_type index = Size::sizeToIndex(size);

    spans[index].lock();

    Span * span = getFreeSpan(size);
    if (span == nullptr) {
        spans[index].unlock();
        return nullptr;
    }

    FreeObject * head = nullptr;
    for (size_type i = 0; i < count; ++i) {
        FreeObject * new_object = span->getFreeList()->front();
        span->getFreeList()->popFront();
        new_object->setNext(head);
        head = new_object;
        span->setUsed(span->used() + 1);

        if (span->getFreeList()->empty()) {
            break;
        }
    }

    spans[index].unlock();

    return head;
}

void CentralCache::returnRange(size_type size, FreeObject * free_object)
{
    size_type index = Size::sizeToIndex(size);

    spans[index].lock();

    while (free_object != nullptr) {
        FreeObject * next = free_object->next();

        void * ptr = reinterpret_cast<void *>(free_object);

        spans[index].unlock();
        Span * span = PageCache::getPageCache().objectToSpan(ptr);
        spans[index].lock();

        if (span == nullptr) {
            free_object = next;
            continue;
        }

        span->getFreeList()->pushFront(free_object);
        span->setUsed(span->used() - 1);

        if (span->used() == 0) {
            spans[index].erase(span);
            span->getFreeList()->clear();

            spans[index].unlock();
            PageCache::getPageCache().returnSpan(span);
            spans[index].lock();
        }

        free_object = next;
    }

    spans[index].unlock();
}

Span * CentralCache::getFreeSpan(size_type size)
{
    size_type index = Size::sizeToIndex(size);

    for (auto it = spans[index].begin(); it != spans[index].end(); ++it) {
        if (!it->getFreeList()->empty()) {
            return &*it;
        }
    }

    spans[index].unlock();

    size_type total_size = size * MAX_BLOCK_NUM;
    size_type page_count = total_size / PAGE_SIZE;
    if (total_size % PAGE_SIZE != 0) {
        page_count += 1;
    }

    if (page_count > MAX_PAGE_NUM) {
        page_count = MAX_PAGE_NUM;
    }

    Span * span = PageCache::getPageCache().fetchSpan(page_count);
    if (span == nullptr) {
        return nullptr;
    }

    void * ptr = Span::idToPtr(span->pageId());

    size_type block_num = span->pageCount() * PAGE_SIZE / size;
    for (size_type i = 0; i < block_num; ++i) {
        void * block_ptr = static_cast<char *>(ptr) + i * size;
        FreeObject * free_obj = reinterpret_cast<FreeObject *>(block_ptr);
        span->getFreeList()->pushFront(free_obj);
    }

    spans[index].lock();
    spans[index].pushFront(span);

    return span;
}

} // namespace vectordb::utils
