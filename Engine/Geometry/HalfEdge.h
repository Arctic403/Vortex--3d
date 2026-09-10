#pragma once
#include <cstdint>
#include <limits>

namespace Vortex
{
inline constexpr uint32_t InvalidIndex = std::numeric_limits<uint32_t>::max();

struct HalfEdge
{
    uint32_t vertex = InvalidIndex;
    uint32_t next = InvalidIndex;
    uint32_t twin = InvalidIndex;
    uint32_t face = InvalidIndex;

    [[nodiscard]] bool HasVertex() const noexcept { return vertex != InvalidIndex; }
    [[nodiscard]] bool HasNext() const noexcept { return next != InvalidIndex; }
    [[nodiscard]] bool HasTwin() const noexcept { return twin != InvalidIndex; }
    [[nodiscard]] bool HasFace() const noexcept { return face != InvalidIndex; }
};
}
