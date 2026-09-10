#pragma once
#include "../Math/Vector3.h"
#include "../Math/Quaternion.h"

namespace Vortex
{
struct Transform
{
    Vector3 position;
    Quaternion rotation;
    Vector3 scale{1.0F, 1.0F, 1.0F};
};
}
