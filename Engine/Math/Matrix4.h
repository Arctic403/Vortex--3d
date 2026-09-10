#pragma once

namespace Vortex
{
struct Matrix4
{
    float data[16]{};

    static Matrix4 Identity()
    {
        Matrix4 result;
        result.data[0] = 1;
        result.data[5] = 1;
        result.data[10] = 1;
        result.data[15] = 1;
        return result;
    }
};
}
