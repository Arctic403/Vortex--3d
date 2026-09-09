#pragma once
#include <vector>
#include "Node.h"

class Scene
{
public:
    std::vector<Node*> nodes;

    ~Scene()
    {
        for(auto* node : nodes)
            delete node;
    }

    Node* CreateNode()
    {
        Node* node = new Node();
        nodes.push_back(node);
        return node;
    }
};
