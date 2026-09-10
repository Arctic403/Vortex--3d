#pragma once

#include <cmath>
#include <cstdint>
#include <unordered_set>

#include "Mesh.h"

namespace Vortex
{
class MeshValidation
{
public:
    [[nodiscard]] static bool Valid(const Mesh& mesh)
    {
        for (const MeshVertex& vertex : mesh.vertices)
        {
            if (!Finite(vertex.position) || !Finite(vertex.normal))
                return false;
        }

        std::unordered_set<uint64_t> edgeKeys;
        for (const MeshEdge& edge : mesh.edges)
        {
            if (edge.a >= mesh.vertices.size() ||
                edge.b >= mesh.vertices.size() ||
                edge.a == edge.b)
                return false;

            if (!edgeKeys.insert(EdgeKey(edge.a, edge.b)).second)
                return false;
        }

        for (const MeshFace& face : mesh.faces)
        {
            if (face.vertices.size() < 3)
                return false;

            std::unordered_set<uint32_t> unique;
            for (uint32_t index : face.vertices)
            {
                if (index >= mesh.vertices.size() || !unique.insert(index).second)
                    return false;
            }


            for (std::size_t i = 0; i < face.vertices.size(); ++i)
            {
                const uint32_t a = face.vertices[i];
                const uint32_t b = face.vertices[(i + 1) % face.vertices.size()];
                if (!edgeKeys.contains(EdgeKey(a, b)))
                    return false;
            }
        }
        return true;
    }

private:
    [[nodiscard]] static bool Finite(const Vector3& value)
    {
        return std::isfinite(value.x) &&
               std::isfinite(value.y) &&
               std::isfinite(value.z);
    }

    [[nodiscard]] static uint64_t EdgeKey(uint32_t a, uint32_t b) noexcept
    {
        const uint32_t low = a < b ? a : b;
        const uint32_t high = a < b ? b : a;
        return (static_cast<uint64_t>(low) << 32U) | high;
    }
};
}
