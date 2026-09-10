#pragma once
#include <cstdint>

namespace Vortex
{
struct HalfEdge
{
    uint32_t vertex = 0;
    uint32_t next = 0;
    uint32_t twin = 0;
    uint32_t face = 0;
};
}
