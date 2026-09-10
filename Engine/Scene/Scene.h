#pragma once
#include <memory>
#include <vector>
#include "Node.h"

namespace Vortex
{
class Scene
{
public:
    Node* CreateNode()
    {
        auto node = std::make_unique<Node>();
        Node* result = node.get();
        nodes.push_back(std::move(node));
        return result;
    }

private:
    std::vector<std::unique_ptr<Node>> nodes;
};
}
