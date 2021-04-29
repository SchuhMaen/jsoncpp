#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include <memory_resource>

namespace Json
{

class SecureResource : public std::pmr::memory_resource
{
public:
    explicit SecureResource(std::pmr::memory_resource* upstream = std::pmr::get_default_resource());
private:
    virtual void* do_allocate(std::size_t bytes, std::size_t alignment) override;
    virtual void do_deallocate(void* storage, std::size_t bytes, std::size_t alignment) override;
    virtual bool do_is_equal(const std::pmr::memory_resource& other_resourece) const noexcept override;

    std::pmr::memory_resource* upstream;
};

}

#endif // __RESOURCE_H__