#include "vectordb/utils/memory_pool/common/platform.h"

#if defined(_WIN32) || defined(_WIN64)
#include <malloc.h>
#elif defined(__linux__)
#include <cstdlib>
#endif

namespace vectordb::utils
{

void * Platform::alignedMalloc(size_type alignment, size_type size)
{
#if defined(_WIN32) || defined(_WIN64)
    return _aligned_malloc(size, alignment);
#elif defined(__linux__)
    return aligned_alloc(alignment, size);
#endif
}

void Platform::alignedFree(void * ptr)
{
#if defined(_WIN32) || defined(_WIN64)
    _aligned_free(ptr);
#elif defined(__linux__)
    free(ptr);
#endif
}

} // namespace vectordb::utils
