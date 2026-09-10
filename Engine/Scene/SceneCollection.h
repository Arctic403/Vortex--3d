#pragma once

#include <string>
#include <utility>
#include <vector>

#include "../Core/Object.h"

namespace Vortex
{
class Node;
class Scene;

class SceneCollection final : public Object
{
public:
    [[nodiscard]] const std::string& Name() const noexcept { return name; }
    void SetName(std::string value) { name = std::move(value); }

    [[nodiscard]] SceneCollection* Parent() noexcept { return parent; }
    [[nodiscard]] const SceneCollection* Parent() const noexcept { return parent; }

    [[nodiscard]] const std::vector<SceneCollection*>& Children() const noexcept
    {
        return children;
    }

    [[nodiscard]] const std::vector<Node*>& Nodes() const noexcept
    {
        return nodes;
    }

    [[nodiscard]] bool IsVisible() const noexcept { return visible; }
    [[nodiscard]] bool IsSelectable() const noexcept { return selectable; }
    [[nodiscard]] bool IsRenderable() const noexcept { return renderable; }

    void SetVisible(bool value) noexcept { visible = value; }
    void SetSelectable(bool value) noexcept { selectable = value; }
    void SetRenderable(bool value) noexcept { renderable = value; }

    [[nodiscard]] bool IsEffectivelyVisible() const noexcept
    {
        for (const SceneCollection* current = this;
             current != nullptr;
             current = current->parent)
        {
            if (!current->visible)
                return false;
        }
        return true;
    }

    [[nodiscard]] bool IsEffectivelySelectable() const noexcept
    {
        for (const SceneCollection* current = this;
             current != nullptr;
             current = current->parent)
        {
            if (!current->visible || !current->selectable)
                return false;
        }
        return true;
    }

    [[nodiscard]] bool IsEffectivelyRenderable() const noexcept
    {
        for (const SceneCollection* current = this;
             current != nullptr;
             current = current->parent)
        {
            if (!current->renderable)
                return false;
        }
        return true;
    }

private:
    friend class Scene;

    explicit SceneCollection(std::string value)
        : name(std::move(value))
    {
    }

    std::string name;
    SceneCollection* parent = nullptr;
    std::vector<SceneCollection*> children;
    std::vector<Node*> nodes;
    bool visible = true;
    bool selectable = true;
    bool renderable = true;
};
}
