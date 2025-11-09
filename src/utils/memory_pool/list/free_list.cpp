#include "vectordb/utils/memory_pool/list/free_list.h"

namespace vectordb::utils
{

FreeObject::FreeObject()
    : next_ptr(nullptr)
{
}

FreeObject::FreeObject(FreeObject * _Next)
    : next_ptr(_Next)
{
}

FreeObject * FreeObject::next() const noexcept
{
    return next_ptr;
}

void FreeObject::setNext(FreeObject * next) noexcept
{
    this->next_ptr = next;
}

FreeListIterator::FreeListIterator(FreeObject * free_object) noexcept
    : free_object(free_object)
{
}

bool FreeListIterator::operator==(const FreeListIterator & other) const noexcept
{
    return free_object == other.free_object;
}

bool FreeListIterator::operator!=(const FreeListIterator & other) const noexcept
{
    return free_object != other.free_object;
}

FreeObject * FreeListIterator::operator*() noexcept
{
    return free_object;
}

FreeObject * FreeListIterator::operator->() noexcept
{
    return free_object;
}

FreeListIterator & FreeListIterator::operator++() noexcept
{
    free_object = free_object->next();
    return *this;
}

FreeListIterator FreeListIterator::operator++(int) noexcept
{
    FreeListIterator tmp = *this;
    ++*this;
    return tmp;
}

FreeList::FreeList()
    : head()
    , size_num(0)
    , max_size(1)
{
}

FreeObject * FreeList::front() noexcept
{
    return head.next();
}

void FreeList::pushFront(FreeObject * free_object)
{
    free_object->setNext(head.next());
    head.setNext(free_object);
    ++size_num;
}

void FreeList::popFront()
{
    FreeObject * next = head.next()->next();
    head.setNext(next);
    --size_num;
}

FreeList::iterator FreeList::begin() noexcept
{
    return iterator(head.next());
}

FreeList::iterator FreeList::end() noexcept
{
    return iterator(nullptr);
}

bool FreeList::empty() const noexcept
{
    return (head.next() == nullptr);
}

size_type FreeList::size() const noexcept
{
    return size_num;
}

size_type FreeList::maxSize() const noexcept
{
    return max_size;
}

void FreeList::setMaxSize(size_type max_size) noexcept
{
    this->max_size = max_size;
}

void FreeList::clear() noexcept
{
    head.setNext(nullptr);
    size_num = 0;
    max_size = 1;
}

} // namespace vectordb::utils
