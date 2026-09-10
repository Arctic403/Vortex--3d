#pragma once
#include <cstdint>
#include <atomic>

namespace Vortex
{
class UUID
{
public:
    UUID() : id(++counter) {}
    explicit UUID(uint64_t value) : id(value) {}

    uint64_t Value() const { return id; }

private:
    uint64_t id = 0;
    inline static std::atomic<uint64_t> counter{0};
};
}
