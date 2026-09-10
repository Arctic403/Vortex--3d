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
        uint32_t edge = InvalidIndex;
    };

    struct Face
    {
        uint32_t edge = InvalidIndex;
    };

    uint32_t AddVertex()
    {
        vertices.push_back({});
        return static_cast<uint32_t>(vertices.size() - 1);
    }

    uint32_t AddFace()
    {
        faces.push_back({});
        return static_cast<uint32_t>(faces.size() - 1);
    }

    uint32_t AddEdge(const HalfEdge& edge)
    {
        edges.push_back(edge);
        return static_cast<uint32_t>(edges.size() - 1);
    }

    void SetTwin(uint32_t a, uint32_t b)
    {
        if (ValidEdge(a) && ValidEdge(b))
        {
            edges[a].twin = b;
            edges[b].twin = a;
        }
    }

    void SetNext(uint32_t edge, uint32_t next)
    {
        if (ValidEdge(edge) && ValidEdge(next))
            edges[edge].next = next;
    }

    void SetFace(uint32_t edge, uint32_t face)
    {
        if (ValidEdge(edge) && face < faces.size())
            edges[edge].face = face;
    }

    bool ValidEdge(uint32_t index) const
    {
        return index < edges.size();
    }

    bool ValidateTopology() const
    {
        for (const auto& edge : edges)
        {
            if (edge.HasTwin() && edge.twin >= edges.size())
                return false;

            if (edge.HasNext() && edge.next >= edges.size())
                return false;

            if (edge.HasFace() && edge.face >= faces.size())
                return false;
        }

        return true;
    }

    const std::vector<Vertex>& Vertices() const { return vertices; }
    const std::vector<HalfEdge>& Edges() const { return edges; }
    const std::vector<Face>& Faces() const { return faces; }

private:
    std::vector<Vertex> vertices;
    std::vector<HalfEdge> edges;
    std::vector<Face> faces;
};
}
