#pragma once

#include <mutex>

#include "vectordb/utils/memory_pool/list/free_list.h"
#include "vectordb/utils/memory_pool/common/common.h"

namespace vectordb::utils
{

/**
 * @brief 页段
 * @details 维护一大段连续的内存
 */
class Span
{
private:
    FreeList free_list;            // 空闲内存块
    size_type page_id;             // 页段号
    Span * prev_ptr;               // 前一个页段
    Span * next_ptr;               // 后一个页段
    size_type page_count;          // 页数
    size_type used_num;            // 已使用的内存块数

public:
    Span();

    ~Span() = default;

public:
    /**
    * @brief 获取页号
    */
    size_type pageId() const noexcept;

    /**
    * @brief 设置页号
    */
    void setPageId(size_type page_id) noexcept;

    /**
    * @brief 获取页数
    */
    size_type pageCount() const noexcept;

    /**
    * @brief 设置页数
    */
    void setPageCount(size_type page_count) noexcept;

    /**
    * @brief 获取上一个页段
    */
    Span * prev() const noexcept;

    /**
    * @brief 设置上一个页段
    */
    void setPrev(Span * prev_span) noexcept;

    /**
    * @brief 获取下一个页段
    */
    Span * next() const noexcept;

    /**
    * @brief 设置下一个页段
    */
    void setNext(Span * next_span) noexcept;

    /**
    * @brief 获取空闲内存块数量
    */
    size_type used() const noexcept;

    /**
    * @brief 设置空闲内存块数量
    */
    void setUsed(size_type used) noexcept;

    /**
    * @brief 获取空闲内存块链表
    */
    FreeList * getFreeList() noexcept;

    /**
    * @brief 将内存地址转为页号
    */
    static size_type ptrToId(void * ptr) noexcept;

    /**
    * @brief 将内存地址转为页号
    */
    static void * idToPtr(size_type id) noexcept;
};

/**
* @brief 页段链表迭代器
*/
class SpanListIterator
{
private:
    Span * span;      // 页段指针

public:
    explicit SpanListIterator(Span * span) noexcept;

    ~SpanListIterator() = default;

public:
    /**
    * @brief 迭代器是否相等
    */
    bool operator==(const SpanListIterator & other) const noexcept;

    /**
    * @brief 迭代器是否不相等
    */
    bool operator!=(const SpanListIterator & other) const noexcept;

    /**
    * @brief 解引用迭代器
    */
    Span & operator*() noexcept;

    /**
    * @brief 解引用迭代器
    */
    Span * operator->() noexcept;

    /**
    * @brief 向后移动
    */
    SpanListIterator & operator++() noexcept;

    /**
    * @brief 向后移动
    */
    SpanListIterator operator++(int) noexcept;

    /**
    * @brief 向前移动
    */
    SpanListIterator & operator--() noexcept;

    /**
    * @brief 向前移动
    */
    SpanListIterator operator--(int) noexcept;
};

/**
* @brief 页段链表
* @details 双向链表，持有一个互斥量，用于在中心缓存中的多线程访问
*/
class SpanList
{
public:
    using iterator = SpanListIterator;

private:
    Span head;                         // 虚拟头节点
    std::mutex mutex;                  // 链表递归锁

public:
    SpanList();

    ~SpanList() = default;

public:
    /**
    * @brief 获取第一个页段
    */
    Span & front() noexcept;

    /**
    * @brief 获取最后一个页段
    */
    Span & back() noexcept;

    /**
    * @brief 获取链表头部
    */
    iterator begin() noexcept;

    /**
    * @brief 获取链表尾部
    */
    iterator end() noexcept;

    /**
    * @brief 页段链表是否为空
    */
    bool empty() const noexcept;

    /**
    * @brief 将页段插入到头部
    */
    void pushFront(Span * span) noexcept;

    /**
    * @brief 从头部删除页段
    */
    void popFront() noexcept;

    /**
    * @brief 删除指定页段
    * @param span 要删除的页段
    */
    void erase(Span * span) noexcept;

    /**
    * @brief 给页段加锁
    */
    void lock() noexcept;

    /**
    * @brief 给页段解锁
    */
    void unlock() noexcept;
};

} // namespace vectordb::utils
