#pragma once

#include <cstdint>
#include <vector>

#include "../Math/Vector3.h"

namespace Vortex
{
struct MeshVertex
{
    Vector3 position;
    Vector3 normal;
};

struct MeshEdge
{
    uint32_t a = 0;
    uint32_t b = 0;
};

struct MeshFace
{
    std::vector<uint32_t> vertices;
};

class Mesh
{
public:
    std::vector<MeshVertex> vertices;
    std::vector<MeshEdge> edges;
    std::vector<MeshFace> faces;
};
}
