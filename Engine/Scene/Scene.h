#pragma once
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "Node.h"

namespace Vortex
{
class Scene
{
public:
    [[nodiscard]] Node* CreateNode(std::string name = {}, Node* parent = nullptr)
    {
        if (parent != nullptr && !Contains(parent))
            return nullptr;

        auto node = std::make_unique<Node>();
        node->name = std::move(name);
        node->parent = parent;
        Node* result = node.get();
        nodes.push_back(std::move(node));

        if (parent != nullptr)
            parent->children.push_back(result);

        return result;
    }

    [[nodiscard]] bool Contains(const Node* candidate) const noexcept
    {
        for (const auto& node : nodes)
        {
            if (node.get() == candidate)
                return true;
        }
        return false;
    }

    [[nodiscard]] const std::vector<std::unique_ptr<Node>>& Nodes() const noexcept
    {
        return nodes;
    }

private:
    std::vector<std::unique_ptr<Node>> nodes;
};
}
