#include "vectordb/utils/memory_pool/cache/page_cache.h"

#include "vectordb/utils/memory_pool/list/span_list.h"
#include "vectordb/utils/memory_pool/common/platform.h"

namespace vectordb::utils
{

PageCache::PageCache()
    : spans()
    , free_span_map()
    , busy_span_map()
    , align_pointers()
    , mutex()
{
}

PageCache::~PageCache()
{
    std::lock_guard<std::mutex> lock(mutex);

    // 释放所有页段
    for (size_type i = 0; i < MAX_PAGE_NUM; ++i) {
        while (!spans[i].empty()) {
            Span & span = spans[i].front();
            spans[i].popFront();
            delete &span;
        }
    }

    // 释放所有对齐指针
    for (void * ptr : align_pointers) {
        Platform::alignedFree(ptr);
    }
}

PageCache & PageCache::getPageCache()
{
    static PageCache instance;
    return instance;
}

Span * PageCache::fetchSpan(size_type pages)
{
    std::lock_guard<std::mutex> lock(mutex);

    if (!spans[pages - 1].empty()) {
        Span & span = spans[pages - 1].front();
        spans[pages - 1].popFront();

        free_span_map.erase(span.pageId());
        free_span_map.erase(span.pageId() + span.pageCount() - 1);

        busy_span_map[span.pageId()] = &span;
        busy_span_map[span.pageId() + span.pageCount() - 1] = &span;

        return &span;
    }

    for (size_type i = pages; i < MAX_PAGE_NUM; ++i) {
        if (!spans[i].empty()) {
            Span & bigger_span = spans[i].front();
            spans[i].popFront();

            Span * split_span = new Span();
            split_span->setPageId(bigger_span.pageId() + i + 1 - pages);
            split_span->setPageCount(pages);

            bigger_span.setPageCount(i + 1 - pages);

            spans[bigger_span.pageCount() - 1].pushFront(&bigger_span);

            free_span_map.erase(bigger_span.pageId() + bigger_span.pageCount() + split_span->pageCount() - 1);
            free_span_map[bigger_span.pageId() + bigger_span.pageCount() - 1] = &bigger_span;

            busy_span_map[split_span->pageId()] = split_span;
            busy_span_map[split_span->pageId() + split_span->pageCount() - 1] = split_span;

            return split_span;
        }
    }

    void * ptr = fetchFromSystem(MAX_PAGE_NUM);
    if (ptr == nullptr) {
        return nullptr;
    }

    align_pointers.emplace_back(ptr);

    Span * max_span = new Span();

    if (pages == MAX_PAGE_NUM) {
        max_span->setPageId(Span::ptrToId(ptr));
        max_span->setPageCount(MAX_PAGE_NUM);

        busy_span_map[max_span->pageId()] = max_span;
        busy_span_map[max_span->pageId() + max_span->pageCount() - 1] = max_span;

        return max_span;
    }

    max_span->setPageId(Span::ptrToId(ptr));
    max_span->setPageCount(MAX_PAGE_NUM - pages);

    Span * split_span = new Span();
    split_span->setPageId(max_span->pageId() + MAX_PAGE_NUM - pages);
    split_span->setPageCount(pages);

    spans[max_span->pageCount() - 1].pushFront(max_span);

    free_span_map[max_span->pageId()] = max_span;
    free_span_map[max_span->pageId() + max_span->pageCount() - 1] = max_span;

    busy_span_map[split_span->pageId()] = split_span;
    busy_span_map[split_span->pageId() + split_span->pageCount() - 1] = split_span;

    return split_span;
}

void PageCache::returnSpan(Span * span)
{
    std::lock_guard<std::mutex> lock(mutex);

    busy_span_map.erase(span->pageId());
    busy_span_map.erase(span->pageId() + span->pageCount() - 1);

    auto prev_it = free_span_map.find(span->pageId() - 1);
    while (prev_it != free_span_map.end()) {
        Span * prev_span = prev_it->second;

        if (span->pageCount() + prev_span->pageCount() > MAX_PAGE_NUM) {
            break;
        }

        spans[prev_span->pageCount() - 1].erase(prev_span);

        free_span_map.erase(prev_span->pageId());
        free_span_map.erase(prev_span->pageId() + prev_span->pageCount() - 1);

        span->setPageId(prev_span->pageId());
        span->setPageCount(prev_span->pageCount() + span->pageCount());

        delete prev_span;

        prev_it = free_span_map.find(span->pageId() - 1);
    }

    auto next_it = free_span_map.find(span->pageId() + span->pageCount());
    while (next_it != free_span_map.end()) {
        Span * next_span = next_it->second;

        if (span->pageCount() + next_span->pageCount() > MAX_PAGE_NUM) {
            break;
        }

        spans[next_span->pageCount() - 1].erase(next_span);

        free_span_map.erase(next_span->pageId());
        free_span_map.erase(next_span->pageId() + next_span->pageCount() - 1);

        span->setPageCount(next_span->pageCount() + span->pageCount());

        delete next_span;

        next_it = free_span_map.find(span->pageId() + span->pageCount());
    }

    free_span_map[span->pageId()] = span;
    free_span_map[span->pageId() + span->pageCount() - 1] = span;

    spans[span->pageCount() - 1].pushFront(span);
}

Span * PageCache::objectToSpan(void * ptr) noexcept
{
    size_type page_id = Span::ptrToId(ptr);

    std::lock_guard<std::mutex> lock(mutex);

    auto it = busy_span_map.upper_bound(page_id);

    if (it == busy_span_map.begin()) {
        return nullptr;
    }

    --it;
    Span * span = it->second;

    if (page_id >= span->pageId() + span->pageCount()) {
        return nullptr;
    }

    return span;
}

void * PageCache::fetchFromSystem(size_type pages) const noexcept
{
    return Platform::alignedMalloc(PAGE_SIZE, pages << PAGE_SHIFT);
}

} // namespace vectordb::utils
