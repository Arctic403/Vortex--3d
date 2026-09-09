#include "mesh.h"

void Mesh::createCube(float size)
{
    float s = size / 2.0f;

    vertices = {
        {-s,-s,-s},{s,-s,-s},{s,s,-s},{-s,s,-s},
        {-s,-s,s},{s,-s,s},{s,s,s},{-s,s,s}
    };

    faces = {
        {0,1,2},{0,2,3},
        {4,6,5},{4,7,6},
        {0,4,5},{0,5,1},
        {3,2,6},{3,6,7},
        {1,5,6},{1,6,2},
        {0,3,7},{0,7,4}
    };
}
