#pragma once
#include <cstdint>
#include <limits>

namespace Vortex
{
constexpr uint32_t InvalidIndex = std::numeric_limits<uint32_t>::max();

struct HalfEdge
{
    uint32_t vertex = InvalidIndex;
    uint32_t next = InvalidIndex;
    uint32_t twin = InvalidIndex;
    uint32_t face = InvalidIndex;

    bool HasNext() const { return next != InvalidIndex; }
    bool HasTwin() const { return twin != InvalidIndex; }
    bool HasFace() const { return face != InvalidIndex; }
};
}
