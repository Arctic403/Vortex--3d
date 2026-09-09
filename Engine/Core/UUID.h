#pragma once
#include <cstdint>
#include <atomic>

class UUID
{
public:
    UUID() : value(++counter) {}
    explicit UUID(uint64_t v) : value(v) {}

    uint64_t Value() const { return value; }

private:
    uint64_t value = 0;
    inline static std::atomic<uint64_t> counter{0};
};
