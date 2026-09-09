#pragma once
#include <string>
#include <vector>
#include "Transform.h"

class Node
{
public:
    std::string name;
    Transform transform;
    Node* parent = nullptr;
    std::vector<Node*> children;
};
