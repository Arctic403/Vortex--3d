#pragma once
#include <vector>
#include "../Math/Vector3.h"

struct Vertex
{
    Vector3 position;
    Vector3 normal;
};

struct Edge
{
    unsigned int a = 0;
    unsigned int b = 0;
};

struct Face
{
    std::vector<unsigned int> vertices;
};

class Mesh
{
public:
    std::vector<Vertex> vertices;
    std::vector<Edge> edges;
    std::vector<Face> faces;
};
