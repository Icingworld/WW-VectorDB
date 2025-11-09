#include "vectordb/utils/memory_pool/list/span_list.h"

namespace vectordb::utils
{

Span::Span()
    : free_list()
    , page_id(0)
    , prev_ptr(nullptr)
    , next_ptr(nullptr)
    , page_count(0)
    , used_num(0)
{
}

size_type Span::pageId() const noexcept
{
    return page_id;
}

void Span::setPageId(size_type id) noexcept
{
    page_id = id;
}

size_type Span::pageCount() const noexcept
{
    return page_count;
}

void Span::setPageCount(size_type count) noexcept
{
    page_count = count;
}

Span * Span::prev() const noexcept
{
    return prev_ptr;
}

void Span::setPrev(Span * prev_span) noexcept
{
    prev_ptr = prev_span;
}

Span * Span::next() const noexcept
{
    return next_ptr;
}

void Span::setNext(Span * next_span) noexcept
{
    next_ptr = next_span;
}

size_type Span::used() const noexcept
{
    return used_num;
}

void Span::setUsed(size_type used) noexcept
{
    used_num = used;
}

FreeList * Span::getFreeList() noexcept
{
    return &free_list;
}

size_type Span::ptrToId(void * ptr) noexcept
{
    return reinterpret_cast<std::uintptr_t>(ptr) >> PAGE_SHIFT;
}

void * Span::idToPtr(size_type id) noexcept
{
    return reinterpret_cast<void *>(id << PAGE_SHIFT);
}

SpanListIterator::SpanListIterator(Span * span) noexcept
    : span(span)
{
}

bool SpanListIterator::operator==(const SpanListIterator & other) const noexcept
{
    return span == other.span;
}

bool SpanListIterator::operator!=(const SpanListIterator & other) const noexcept
{
    return span != other.span;
}

Span & SpanListIterator::operator*() noexcept
{
    return *span;
}

Span * SpanListIterator::operator->() noexcept
{
    return span;
}

SpanListIterator & SpanListIterator::operator++() noexcept
{
    span = span->next();
    return *this;
}

SpanListIterator SpanListIterator::operator++(int) noexcept
{
    SpanListIterator tmp = *this;
    ++(*this);
    return tmp;
}

SpanListIterator & SpanListIterator::operator--() noexcept
{
    span = span->prev();
    return *this;
}

SpanListIterator SpanListIterator::operator--(int) noexcept
{
    SpanListIterator tmp = *this;
    --(*this);
    return tmp;
}

SpanList::SpanList()
    : head()
    , mutex()
{
    head.setNext(&head);
    head.setPrev(&head);
}

Span & SpanList::front() noexcept
{
    return *head.next();
}

Span & SpanList::back() noexcept
{
    return *head.prev();
}

SpanList::iterator SpanList::begin() noexcept
{
    return iterator(head.next());
}

SpanList::iterator SpanList::end() noexcept
{
    return iterator(&head);
}

void SpanList::pushFront(Span * span_ptr) noexcept
{
    Span * next_span = head.next();
    span_ptr->setNext(next_span);
    span_ptr->setPrev(&head);
    next_span->setPrev(span_ptr);
    head.setNext(span_ptr);
}

void SpanList::popFront() noexcept
{
    Span * front_span = head.next();
    head.setNext(front_span->next());
    front_span->next()->setPrev(&head);
}

void SpanList::erase(Span * span_ptr) noexcept
{
    Span * prev_span = span_ptr->prev();
    Span * next_span = span_ptr->next();
    prev_span->setNext(next_span);
    next_span->setPrev(prev_span);
}

bool SpanList::empty() const noexcept
{
    return head.next() == &head;
}

void SpanList::lock() noexcept
{
    mutex.lock();
}

void SpanList::unlock() noexcept
{
    mutex.unlock();
}

} // namespace vectordb::utils
