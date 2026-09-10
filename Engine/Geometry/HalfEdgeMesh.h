#pragma once

#include <cstdint>
#include <string>
#include <unordered_set>
#include <vector>

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

    [[nodiscard]] uint32_t AddVertex()
    {
        if (vertices.size() >= InvalidIndex)
            return InvalidIndex;
        vertices.push_back({});
        return static_cast<uint32_t>(vertices.size() - 1);
    }

    [[nodiscard]] uint32_t AddFace()
    {
        if (faces.size() >= InvalidIndex)
            return InvalidIndex;
        faces.push_back({});
        return static_cast<uint32_t>(faces.size() - 1);
    }

    [[nodiscard]] uint32_t AddEdge(uint32_t vertex)
    {
        if (!ValidVertex(vertex) || edges.size() >= InvalidIndex)
            return InvalidIndex;

        const auto index = static_cast<uint32_t>(edges.size());
        HalfEdge edge;
        edge.vertex = vertex;
        edges.push_back(edge);

        if (vertices[vertex].edge == InvalidIndex)
            vertices[vertex].edge = index;
        return index;
    }

    [[nodiscard]] bool SetVertex(uint32_t edge, uint32_t vertex)
    {
        if (!ValidEdge(edge) || !ValidVertex(vertex))
            return false;

        const uint32_t previous = edges[edge].vertex;
        edges[edge].vertex = vertex;
        if (vertices[vertex].edge == InvalidIndex)
            vertices[vertex].edge = edge;
        if (ValidVertex(previous) && previous != vertex &&
            vertices[previous].edge == edge)
            RefreshVertexEdge(previous);
        return true;
    }

    [[nodiscard]] bool SetTwin(uint32_t a, uint32_t b)
    {
        if (!ValidEdge(a) || !ValidEdge(b) || a == b)
            return false;

        (void)ClearTwin(a);
        (void)ClearTwin(b);
        edges[a].twin = b;
        edges[b].twin = a;
        return true;
    }

    [[nodiscard]] bool ClearTwin(uint32_t edge)
    {
        if (!ValidEdge(edge))
            return false;

        const uint32_t twin = edges[edge].twin;
        edges[edge].twin = InvalidIndex;
        if (ValidEdge(twin) && edges[twin].twin == edge)
            edges[twin].twin = InvalidIndex;
        return true;
    }

    [[nodiscard]] bool SetNext(uint32_t edge, uint32_t next)
    {
        if (!ValidEdge(edge))
            return false;
        if (next == InvalidIndex)
        {
            edges[edge].next = InvalidIndex;
            return true;
        }
        if (!ValidEdge(next) || edge == next)
            return false;

        edges[edge].next = next;
        return true;
    }

    [[nodiscard]] bool SetFace(uint32_t edge, uint32_t face)
    {
        if (!ValidEdge(edge))
            return false;
        if (face != InvalidIndex && !ValidFace(face))
            return false;

        const uint32_t previous = edges[edge].face;
        edges[edge].face = face;
        if (ValidFace(face) && faces[face].edge == InvalidIndex)
            faces[face].edge = edge;
        if (ValidFace(previous) && previous != face && faces[previous].edge == edge)
            RefreshFaceEdge(previous);
        return true;
    }

    [[nodiscard]] bool ValidVertex(uint32_t index) const noexcept
    {
        return index < vertices.size();
    }

    [[nodiscard]] bool ValidEdge(uint32_t index) const noexcept
    {
        return index < edges.size();
    }

    [[nodiscard]] bool ValidFace(uint32_t index) const noexcept
    {
        return index < faces.size();
    }

    [[nodiscard]] bool ValidateTopology(std::string* error = nullptr) const
    {
        auto fail = [error](const std::string& message)
        {
            if (error != nullptr)
                *error = message;
            return false;
        };

        for (uint32_t vertexIndex = 0;
             vertexIndex < static_cast<uint32_t>(vertices.size());
             ++vertexIndex)
        {
            const uint32_t edge = vertices[vertexIndex].edge;
            if (edge != InvalidIndex &&
                (!ValidEdge(edge) || edges[edge].vertex != vertexIndex))
                return fail("vertex representative edge is invalid");
        }

        for (uint32_t edgeIndex = 0;
             edgeIndex < static_cast<uint32_t>(edges.size());
             ++edgeIndex)
        {
            const HalfEdge& edge = edges[edgeIndex];
            if (!ValidVertex(edge.vertex))
                return fail("edge vertex is invalid");
            if (!edge.HasNext())
                return fail("edge has no next edge");
            if (!ValidEdge(edge.next) || edge.next == edgeIndex)
                return fail("edge next reference is invalid");
            if (edges[edge.next].vertex == edge.vertex)
                return fail("edge has identical origin and destination vertices");
            if (edge.HasFace() && !ValidFace(edge.face))
                return fail("edge face reference is invalid");
            if (edges[edge.next].face != edge.face)
                return fail("edge loop crosses between face and boundary regions");

            if (edge.HasTwin())
            {
                if (!ValidEdge(edge.twin) || edge.twin == edgeIndex)
                    return fail("edge twin reference is invalid");
                if (edges[edge.twin].twin != edgeIndex)
                    return fail("edge twin relationship is not reciprocal");

                if (!edges[edge.twin].HasNext() ||
                    !ValidEdge(edges[edge.twin].next))
                    return fail("twinned edge has an invalid next reference");

                const uint32_t destination = edges[edge.next].vertex;
                const uint32_t twinDestination =
                    edges[edges[edge.twin].next].vertex;
                if (edges[edge.twin].vertex != destination ||
                    twinDestination != edge.vertex)
                    return fail("twinned edges do not have opposite endpoints");
            }
        }

        std::vector<uint32_t> predecessorCount(edges.size(), 0);
        for (const HalfEdge& edge : edges)
            ++predecessorCount[edge.next];
        for (uint32_t count : predecessorCount)
        {
            if (count != 1)
                return fail("each edge must have exactly one predecessor");
        }

        std::vector<bool> visited(edges.size(), false);
        for (uint32_t faceIndex = 0;
             faceIndex < static_cast<uint32_t>(faces.size());
             ++faceIndex)
        {
            const uint32_t start = faces[faceIndex].edge;
            if (!ValidEdge(start) || edges[start].face != faceIndex)
                return fail("face representative edge is invalid");

            uint32_t current = start;
            std::size_t edgeCount = 0;
            std::unordered_set<uint32_t> faceVertices;
            do
            {
                if (!ValidEdge(current))
                    return fail("face loop contains an invalid edge");
                if (edges[current].face != faceIndex)
                    return fail("face loop crosses into another face");
                if (visited[current])
                    return fail("edge belongs to multiple or malformed face loops");
                if (!edges[current].HasNext())
                    return fail("face loop is open");
                if (!faceVertices.insert(edges[current].vertex).second)
                    return fail("face loop repeats a vertex");

                visited[current] = true;
                current = edges[current].next;
                ++edgeCount;
                if (edgeCount > edges.size())
                    return fail("face loop does not close");
            } while (current != start);

            if (edgeCount < 3)
                return fail("face has fewer than three edges");
        }

        for (uint32_t edgeIndex = 0;
             edgeIndex < static_cast<uint32_t>(edges.size());
             ++edgeIndex)
        {
            if (edges[edgeIndex].HasFace() && !visited[edgeIndex])
                return fail("face edge is not reachable from its face");
        }

        if (error != nullptr)
            error->clear();
        return true;
    }

    [[nodiscard]] const std::vector<Vertex>& Vertices() const noexcept
    {
        return vertices;
    }

    [[nodiscard]] const std::vector<HalfEdge>& Edges() const noexcept
    {
        return edges;
    }

    [[nodiscard]] const std::vector<Face>& Faces() const noexcept
    {
        return faces;
    }

private:
    void RefreshVertexEdge(uint32_t vertex)
    {
        vertices[vertex].edge = InvalidIndex;
        for (uint32_t i = 0; i < static_cast<uint32_t>(edges.size()); ++i)
        {
            if (edges[i].vertex == vertex)
            {
                vertices[vertex].edge = i;
                return;
            }
        }
    }

    void RefreshFaceEdge(uint32_t face)
    {
        faces[face].edge = InvalidIndex;
        for (uint32_t i = 0; i < static_cast<uint32_t>(edges.size()); ++i)
        {
            if (edges[i].face == face)
            {
                faces[face].edge = i;
                return;
            }
        }
    }

    std::vector<Vertex> vertices;
    std::vector<HalfEdge> edges;
    std::vector<Face> faces;
};
}
