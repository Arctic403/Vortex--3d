#pragma once

#include <vector>
#include <cstdint>
#include "HalfEdge.h"

namespace Vortex
{
class HalfEdgeMesh
{
public:
    struct Vertex
    {
        uint32_t halfEdge = 0;
    };

    struct Face
    {
        uint32_t halfEdge = 0;
    };

    uint32_t AddVertex()
    {
        vertices.push_back({});
        return static_cast<uint32_t>(vertices.size() - 1);
    }

    uint32_t AddHalfEdge(const HalfEdge& edge)
    {
        halfEdges.push_back(edge);
        return static_cast<uint32_t>(halfEdges.size() - 1);
    }

    uint32_t AddFace()
    {
        faces.push_back({});
        return static_cast<uint32_t>(faces.size() - 1);
    }

    const std::vector<Vertex>& Vertices() const
    {
        return vertices;
    }

    const std::vector<HalfEdge>& Edges() const
    {
        return halfEdges;
    }

    const std::vector<Face>& Faces() const
    {
        return faces;
    }

private:
    std::vector<Vertex> vertices;
    std::vector<HalfEdge> halfEdges;
    std::vector<Face> faces;
};
}
