#pragma once

#include "vectordb/utils/memory_pool/common/common.h"

namespace vectordb::utils
{

/**
 * @brief 平台接口
*/
class Platform
{
public:
    /**
     * @brief 从堆中以对齐方法获取内存
     * @param alignment 对齐大小
     * @param size 获取内存大小
    */
    static void * alignedMalloc(size_type alignment, size_type size);

    /**
     * @brief 释放以对齐方法获取的内存
     * @param ptr 释放内存的指针
    */
    static void alignedFree(void * ptr);
};

} // namespace vectordb::utils