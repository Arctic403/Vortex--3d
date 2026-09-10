#pragma once
#include "Mesh.h"

namespace Vortex
{
class MeshValidation
{
public:
    static bool Valid(const Mesh& mesh)
    {
        return !mesh.vertices.empty() || mesh.faces.empty();
    }
};
}
