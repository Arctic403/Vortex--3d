#pragma once
#include <atomic>
#include <cstdint>
#include <limits>
#include <stdexcept>

namespace Vortex
{
class UUID
{
public:
    UUID() : id(Next()) {}

    explicit UUID(uint64_t value) : id(value)
    {
        Reserve(value);
    }

    [[nodiscard]] uint64_t Value() const noexcept { return id; }
    [[nodiscard]] bool IsValid() const noexcept { return id != 0; }

    friend bool operator==(const UUID&, const UUID&) = default;

private:
    static uint64_t Next()
    {
        uint64_t current = counter.load(std::memory_order_relaxed);
        for (;;)
        {
            if (current == std::numeric_limits<uint64_t>::max())
                throw std::overflow_error("UUID counter exhausted");

            if (counter.compare_exchange_weak(
                    current,
                    current + 1,
                    std::memory_order_relaxed,
                    std::memory_order_relaxed))
                return current + 1;
        }
    }

    static void Reserve(uint64_t value) noexcept
    {
        uint64_t current = counter.load(std::memory_order_relaxed);
        while (current < value &&
               !counter.compare_exchange_weak(
                   current,
                   value,
                   std::memory_order_relaxed,
                   std::memory_order_relaxed))
        {
        }
    }

    uint64_t id = 0;
    inline static std::atomic<uint64_t> counter{0};
};
}
