#pragma once

namespace Vortex
{
struct Vector3
{
    float x = 0;
    float y = 0;
    float z = 0;

    Vector3 operator+(const Vector3& other) const
    {
        return {x + other.x, y + other.y, z + other.z};
    }

    Vector3 operator-(const Vector3& other) const
    {
        return {x - other.x, y - other.y, z - other.z};
    }

    Vector3 operator*(float value) const
    {
        return {x * value, y * value, z * value};
    }
};
}
