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
            // 销毁页段
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

    // 如果有空闲页段，直接从链表中直接获取页段
    if (!spans[pages - 1].empty()) {
        // 从链表中取出页段
        Span & span = spans[pages - 1].front();
        spans[pages - 1].popFront();

        // 从空闲映射表中移除页段
        free_span_map.erase(span.pageId());
        free_span_map.erase(span.pageId() + span.pageCount() - 1);

        // 将页段插入到繁忙映射表中
        busy_span_map[span.pageId()] = &span;
        busy_span_map[span.pageId() + span.pageCount() - 1] = &span;

        return &span;
    }

    // 没有正好这么大的页段，尝试从更大块内存中切出页段
    for (size_type i = pages; i < MAX_PAGE_NUM; ++i) {
        if (!spans[i].empty()) {
            // 取出页段，该页段页数为 i + 1
            Span & bigger_span = spans[i].front();
            spans[i].popFront();

            // 切分 i + 1 页的页段，切成 i = (i + 1 - pages) + pages 的两个页段
            // 新建一个 split_span 用于储存后面长 pages 页的页段
            Span * split_span = new Span();
            split_span->setPageId(bigger_span.pageId() + i + 1 - pages);
            split_span->setPageCount(pages);

            // bigger_span 用于储存前面长 i + 1 - pages 的页段，直接修改页数
            bigger_span.setPageCount(i + 1 - pages);

            // 前面的页段插入到对应链表中
            spans[bigger_span.pageCount() - 1].pushFront(&bigger_span);

            // 修改映射表
            // 解除原 bigger_span 的尾页号映射
            free_span_map.erase(bigger_span.pageId() + bigger_span.pageCount() + split_span->pageCount() - 1);
            // 添加新 bigger_span 的尾页号映射
            free_span_map[bigger_span.pageId() + bigger_span.pageCount() - 1] = &bigger_span;

            // split_span 页号插入繁忙映射表
            busy_span_map[split_span->pageId()] = split_span;
            busy_span_map[split_span->pageId() + split_span->pageCount() - 1] = split_span;

            return split_span;
        }
    }

    // 没找到更大的页段，直接申请一个最大的页段，然后按照上面的流程重新获取页段
    void * ptr = fetchFromSystem(MAX_PAGE_NUM);
    if (ptr == nullptr) {
        return nullptr;
    }

    // 记录该对齐指针
    align_pointers.emplace_back(ptr);

    Span * max_span = new Span();

    if (pages == MAX_PAGE_NUM) {
        // 恰好需要最大页数
        max_span->setPageId(Span::ptrToId(ptr));
        max_span->setPageCount(MAX_PAGE_NUM);

        // 建立繁忙表映射
        busy_span_map[max_span->pageId()] = max_span;
        busy_span_map[max_span->pageId() + max_span->pageCount() - 1] = max_span;

        return max_span;
    }

    // max_span 用来储存 MAX_PAGE_NUM - pages 的页段
    max_span->setPageId(Span::ptrToId(ptr));
    max_span->setPageCount(MAX_PAGE_NUM - pages);

    // 新建一个页段用于返回
    Span * split_span = new Span();
    split_span->setPageId(max_span->pageId() + MAX_PAGE_NUM - pages);
    split_span->setPageCount(pages);

    // 新页段插入页段链表中
    spans[max_span->pageCount() - 1].pushFront(max_span);

    // max_span 页号插入空闲映射表
    free_span_map[max_span->pageId()] = max_span;
    free_span_map[max_span->pageId() + max_span->pageCount() - 1] = max_span;

    // split_span页号插入繁忙映射表
    busy_span_map[split_span->pageId()] = split_span;
    busy_span_map[split_span->pageId() + split_span->pageCount() - 1] = split_span;

    return split_span;
}

void PageCache::returnSpan(Span * span)
{
    std::lock_guard<std::mutex> lock(mutex);

    // 从繁忙映射表中删除该页段
    busy_span_map.erase(span->pageId());
    busy_span_map.erase(span->pageId() + span->pageCount() - 1);

    // 向前寻找空闲的页
    auto prev_it = free_span_map.find(span->pageId() - 1);
    while (prev_it != free_span_map.end()) {
        Span * prev_span = prev_it->second;

        // 判断合并后是否超出上限
        if (span->pageCount() + prev_span->pageCount() > MAX_PAGE_NUM) {
            break;
        }

        // 从链表中删除该空闲页
        spans[prev_span->pageCount() - 1].erase(prev_span);

        // 从哈希表中删除该空闲页
        free_span_map.erase(prev_span->pageId());
        free_span_map.erase(prev_span->pageId() + prev_span->pageCount() - 1);

        // 合并页段
        span->setPageId(prev_span->pageId());
        span->setPageCount(prev_span->pageCount() + span->pageCount());

        // 删除原空闲页
        delete prev_span;

        prev_it = free_span_map.find(span->pageId() - 1);
    }

    // 向后寻找空闲的页
    auto next_it = free_span_map.find(span->pageId() + span->pageCount());
    while (next_it != free_span_map.end()) {
        Span * next_span = next_it->second;

        // 判断合并后是否超出上限
        if (span->pageCount() + next_span->pageCount() > MAX_PAGE_NUM) {
            break;
        }

        // 从链表中删除该空闲页
        spans[next_span->pageCount() - 1].erase(next_span);

        // 从哈希表中删除该空闲页
        free_span_map.erase(next_span->pageId());
        free_span_map.erase(next_span->pageId() + next_span->pageCount() - 1);

        // 合并页段
        span->setPageCount(next_span->pageCount() + span->pageCount());

        // 删除原空闲页
        delete next_span;

        next_it = free_span_map.find(span->pageId() + span->pageCount());
    }

    // 添加新页段的映射
    free_span_map[span->pageId()] = span;
    free_span_map[span->pageId() + span->pageCount() - 1] = span;

    // 合并完成，插入新的链表
    spans[span->pageCount() - 1].pushFront(span);
}

Span * PageCache::objectToSpan(void * ptr) noexcept
{
    size_type page_id = Span::ptrToId(ptr);

    std::lock_guard<std::mutex> lock(mutex);

    // 寻找首个大于该页号的迭代器
    auto it = busy_span_map.upper_bound(page_id);

    if (it == busy_span_map.begin()) {
        return nullptr;
    }

    --it;
    Span * span = it->second;

    if (page_id >= span->pageId() + span->pageCount()) {
        // 不在该页段范围内
        return nullptr;
    }

    return span;
}

void * PageCache::fetchFromSystem(size_type pages) const noexcept
{
    return Platform::alignedMalloc(PAGE_SIZE, pages << PAGE_SHIFT);
}

} // namespace vectordb::utils
