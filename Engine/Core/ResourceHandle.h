#pragma once
#include <cstdint>

namespace Vortex
{
template<typename T>
class ResourceHandle
{
public:
    ResourceHandle() = default;

    explicit ResourceHandle(uint64_t value)
        : id(value)
    {
    }

    uint64_t ID() const
    {
        return id;
    }

private:
    uint64_t id = 0;
};
}
