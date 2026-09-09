#pragma once
#include <vector>
#include "../Math/Vector3.h"

struct Vertex
{
    Vector3 position;
};

struct Face
{
    unsigned int a;
    unsigned int b;
    unsigned int c;
};

class Mesh
{
public:
    std::vector<Vertex> vertices;
    std::vector<Face> faces;
};
