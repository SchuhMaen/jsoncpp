#include "json/resource.h"

#include <cstring>

namespace Json
{

SecureResource::SecureResource(std::pmr::memory_resource* upstream)
    : upstream(upstream)
{}

void* SecureResource::do_allocate(std::size_t bytes, std::size_t alignment)
{
    return upstream->allocate(bytes, alignment);
}

void SecureResource::do_deallocate(void* storage, std::size_t bytes, std::size_t alignment)
{
    #ifdef __STDC_LIB_EXT1__
      memset_s(storage, bytes + alignment, 0, bytes + alignment);
    #else
      std::memset(storage, 0, bytes + alignment); // requires -fno-builtin-memset
    #endif
    upstream->deallocate(storage, bytes, alignment);
}

bool SecureResource::do_is_equal(const std::pmr::memory_resource& other_resource) const noexcept
{
    return this == &other_resource;
}

}