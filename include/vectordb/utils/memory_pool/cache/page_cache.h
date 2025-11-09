#pragma once

#include <array>
#include <vector>
#include <unordered_map>
#include <map>

#include "vectordb/utils/memory_pool/list/span_list.h"
#include "vectordb/utils/memory_pool/common/common.h"

namespace vectordb::utils
{

/**
 * @brief 页缓存
 */
class PageCache
{
private:
    std::array<SpanList, MAX_PAGE_NUM> spans;              // 页段链表数组
    std::unordered_map<size_type, Span *> free_span_map;   // 页号到空闲页段指针的映射
    std::map<size_type, Span *> busy_span_map;             // 页号到繁忙页段指针的映射
    std::vector<void *> align_pointers;                    // 对齐指针数组
    std::mutex mutex;                                      // 页缓存锁

private:
    PageCache();

    PageCache(const PageCache &) = delete;

    PageCache & operator=(const PageCache &) = delete;

public:
    ~PageCache();

public:
    /**
    * @brief 获取页缓存单例
    */
    static PageCache & getPageCache();

    /**
    * @brief 获取指定大小的页段
    * @param pages 页数
    * @return 成功时返回`Span *`，失败时返回`nullptr`
    */
    Span * fetchSpan(size_type pages);

    /**
    * @brief 将页段归还到页缓存
    * @param span 页段
    */
    void returnSpan(Span * span);

    /**
    * @brief 通过内存块指针找到对应页段
    * @param ptr 内存块指针
    * @return 成功时返回`Span *`，失败时返回`nullptr`
    */
    Span * objectToSpan(void * ptr) noexcept;

private:
    /**
    * @brief 从系统内存中获取指定大小的内存
    * @param pages 页数
    * @return 成功时返回`void *`，失败时返回`nullptr`
    */
    void * fetchFromSystem(size_type pages) const noexcept;
};

} // namespace vectordb::utils
