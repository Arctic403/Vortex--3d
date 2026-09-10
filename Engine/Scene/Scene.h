#pragma once
#include <vector>
#include "Node.h"

namespace Vortex
{
class Scene
{
public:
    ~Scene()
    {
        for(Node* node : nodes)
            delete node;
    }

    Node* CreateNode()
    {
        Node* node = new Node();
        nodes.push_back(node);
        return node;
    }

private:
    std::vector<Node*> nodes;
};
}
