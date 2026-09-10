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

    [[nodiscard]] uint64_t ID() const noexcept
    {
        return id;
    }

    [[nodiscard]] bool IsValid() const noexcept { return id != 0; }
    explicit operator bool() const noexcept { return IsValid(); }
    friend bool operator==(const ResourceHandle&, const ResourceHandle&) = default;

private:
    uint64_t id = 0;
};
}
