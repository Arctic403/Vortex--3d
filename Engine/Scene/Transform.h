#pragma once
#include "../Math/Vector3.h"
#include "../Math/Quaternion.h"

struct Transform
{
    Vector3 position;
    Quaternion rotation;
    Vector3 scale{1,1,1};
};
