#pragma once
#include <string>
#include <vector>
#include "../Core/Object.h"
#include "Transform.h"

namespace Vortex
{
class Node final : public Object
{
public:
    std::string name;
    Transform transform;

    [[nodiscard]] Node* Parent() noexcept { return parent; }
    [[nodiscard]] const Node* Parent() const noexcept { return parent; }

    [[nodiscard]] const std::vector<Node*>& Children() const noexcept
    {
        return children;
    }

private:
    friend class Scene;

    Node* parent = nullptr;
    std::vector<Node*> children;
};
}
