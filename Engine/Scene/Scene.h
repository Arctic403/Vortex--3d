#pragma once

#include <algorithm>
#include <memory>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include "Node.h"
#include "SceneCollection.h"

namespace Vortex
{
class Scene
{
public:
    Scene()
    {
        auto master = std::unique_ptr<SceneCollection>(
            new SceneCollection("Scene Collection"));
        masterCollection = master.get();
        activeCollection = masterCollection;
        collections.push_back(std::move(master));
    }

    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;
    Scene(Scene&&) = delete;
    Scene& operator=(Scene&&) = delete;
    ~Scene() = default;

    [[nodiscard]] Node* CreateNode(
        std::string name = {},
        Node* parent = nullptr,
        SceneCollection* collection = nullptr)
    {
        if (parent != nullptr && !Contains(parent))
            return nullptr;

        SceneCollection* destination =
            collection == nullptr ? activeCollection : collection;
        if (!Contains(destination))
            return nullptr;

        auto node = std::make_unique<Node>();
        node->name = std::move(name);
        node->parent = parent;
        Node* result = node.get();
        nodes.push_back(std::move(node));

        if (parent != nullptr)
            parent->children.push_back(result);
        destination->nodes.push_back(result);
        return result;
    }

    [[nodiscard]] bool DeleteNode(Node* node)
    {
        if (!Contains(node))
            return false;

        for (auto& collection : collections)
            ErasePointer(collection->nodes, node);

        if (node->parent != nullptr)
            ErasePointer(node->parent->children, node);

        for (Node* child : node->children)
            child->parent = nullptr;
        node->children.clear();

        const auto found = std::find_if(
            nodes.begin(), nodes.end(),
            [node](const std::unique_ptr<Node>& candidate)
            {
                return candidate.get() == node;
            });
        nodes.erase(found);
        return true;
    }

    [[nodiscard]] bool SetNodeParent(Node* node, Node* parent)
    {
        if (!Contains(node) || (parent != nullptr && !Contains(parent)) ||
            node == parent || WouldCreateNodeCycle(node, parent))
            return false;

        if (node->parent == parent)
            return true;

        if (node->parent != nullptr)
            ErasePointer(node->parent->children, node);
        node->parent = parent;
        if (parent != nullptr && !HasPointer(parent->children, node))
            parent->children.push_back(node);
        return true;
    }

    [[nodiscard]] SceneCollection* CreateCollection(
        std::string name,
        SceneCollection* parent = nullptr)
    {
        SceneCollection* destination =
            parent == nullptr ? masterCollection : parent;
        if (!Contains(destination))
            return nullptr;

        auto collection = std::unique_ptr<SceneCollection>(
            new SceneCollection(std::move(name)));
        collection->parent = destination;
        SceneCollection* result = collection.get();
        collections.push_back(std::move(collection));
        destination->children.push_back(result);
        return result;
    }

    [[nodiscard]] bool DeleteCollection(SceneCollection* collection)
    {
        if (!Contains(collection) || collection == masterCollection)
            return false;

        SceneCollection* destination = collection->parent;

        for (Node* node : collection->nodes)
        {
            if (!HasPointer(destination->nodes, node))
                destination->nodes.push_back(node);
        }
        collection->nodes.clear();

        for (SceneCollection* child : collection->children)
        {
            child->parent = destination;
            if (!HasPointer(destination->children, child))
                destination->children.push_back(child);
        }
        collection->children.clear();

        ErasePointer(destination->children, collection);
        if (activeCollection == collection)
            activeCollection = destination;

        const auto found = std::find_if(
            collections.begin(), collections.end(),
            [collection](const std::unique_ptr<SceneCollection>& candidate)
            {
                return candidate.get() == collection;
            });
        collections.erase(found);
        return true;
    }

    [[nodiscard]] bool ReparentCollection(
        SceneCollection* collection,
        SceneCollection* parent)
    {
        if (!Contains(collection) || !Contains(parent) ||
            collection == masterCollection || collection == parent ||
            WouldCreateCollectionCycle(collection, parent))
            return false;

        if (collection->parent == parent)
            return true;

        ErasePointer(collection->parent->children, collection);
        collection->parent = parent;
        if (!HasPointer(parent->children, collection))
            parent->children.push_back(collection);
        return true;
    }

    [[nodiscard]] bool LinkNode(SceneCollection* collection, Node* node)
    {
        if (!Contains(collection) || !Contains(node) ||
            HasPointer(collection->nodes, node))
            return false;

        collection->nodes.push_back(node);
        return true;
    }

    [[nodiscard]] bool UnlinkNode(SceneCollection* collection, Node* node)
    {
        if (!Contains(collection) || !Contains(node))
            return false;
        return ErasePointer(collection->nodes, node);
    }

    [[nodiscard]] bool IsLinked(
        const SceneCollection* collection,
        const Node* node) const noexcept
    {
        return Contains(collection) && Contains(node) &&
               HasPointer(collection->nodes, node);
    }

    [[nodiscard]] std::vector<const SceneCollection*> CollectionsFor(
        const Node* node) const
    {
        std::vector<const SceneCollection*> result;
        if (!Contains(node))
            return result;

        for (const auto& collection : collections)
        {
            if (HasPointer(collection->nodes, node))
                result.push_back(collection.get());
        }
        return result;
    }

    [[nodiscard]] bool SetActiveCollection(SceneCollection* collection) noexcept
    {
        if (!Contains(collection))
            return false;
        activeCollection = collection;
        return true;
    }

    [[nodiscard]] SceneCollection* MasterCollection() noexcept
    {
        return masterCollection;
    }

    [[nodiscard]] const SceneCollection* MasterCollection() const noexcept
    {
        return masterCollection;
    }

    [[nodiscard]] SceneCollection* ActiveCollection() noexcept
    {
        return activeCollection;
    }

    [[nodiscard]] const SceneCollection* ActiveCollection() const noexcept
    {
        return activeCollection;
    }

    [[nodiscard]] bool Contains(const Node* candidate) const noexcept
    {
        return candidate != nullptr && std::any_of(
            nodes.begin(), nodes.end(),
            [candidate](const std::unique_ptr<Node>& node)
            {
                return node.get() == candidate;
            });
    }

    [[nodiscard]] bool Contains(const SceneCollection* candidate) const noexcept
    {
        return candidate != nullptr && std::any_of(
            collections.begin(), collections.end(),
            [candidate](const std::unique_ptr<SceneCollection>& collection)
            {
                return collection.get() == candidate;
            });
    }

    [[nodiscard]] Node* FindNode(UUID id) noexcept
    {
        for (const auto& node : nodes)
        {
            if (node->GetID() == id)
                return node.get();
        }
        return nullptr;
    }

    [[nodiscard]] const Node* FindNode(UUID id) const noexcept
    {
        for (const auto& node : nodes)
        {
            if (node->GetID() == id)
                return node.get();
        }
        return nullptr;
    }

    [[nodiscard]] SceneCollection* FindCollection(UUID id) noexcept
    {
        for (const auto& collection : collections)
        {
            if (collection->GetID() == id)
                return collection.get();
        }
        return nullptr;
    }

    [[nodiscard]] const SceneCollection* FindCollection(UUID id) const noexcept
    {
        for (const auto& collection : collections)
        {
            if (collection->GetID() == id)
                return collection.get();
        }
        return nullptr;
    }

    [[nodiscard]] const std::vector<std::unique_ptr<Node>>& Nodes() const noexcept
    {
        return nodes;
    }

    [[nodiscard]] const std::vector<std::unique_ptr<SceneCollection>>&
    Collections() const noexcept
    {
        return collections;
    }

    [[nodiscard]] bool Validate(std::string* error = nullptr) const
    {
        auto fail = [error](const std::string& message)
        {
            if (error != nullptr)
                *error = message;
            return false;
        };

        if (!Contains(masterCollection) || masterCollection->parent != nullptr)
            return fail("master collection is invalid");
        if (!Contains(activeCollection))
            return fail("active collection is invalid");

        std::unordered_set<uint64_t> objectIds;
        for (const auto& node : nodes)
        {
            if (!node->GetID().IsValid() ||
                !objectIds.insert(node->GetID().Value()).second)
                return fail("node UUID is invalid or duplicated");

            std::unordered_set<const Node*> childSet;
            for (const Node* child : node->children)
            {
                if (!Contains(child) || child->parent != node.get() ||
                    !childSet.insert(child).second)
                    return fail("node child relationship is invalid");
            }

            if (node->parent != nullptr)
            {
                if (!Contains(node->parent) ||
                    PointerCount(node->parent->children, node.get()) != 1)
                    return fail("node parent relationship is invalid");
            }

            std::unordered_set<const Node*> ancestry;
            for (const Node* current = node.get();
                 current != nullptr;
                 current = current->parent)
            {
                if (!Contains(current) || !ancestry.insert(current).second)
                    return fail("node hierarchy contains a cycle");
            }
        }

        for (const auto& collection : collections)
        {
            if (!collection->GetID().IsValid() ||
                !objectIds.insert(collection->GetID().Value()).second)
                return fail("collection UUID is invalid or duplicated");

            std::unordered_set<const SceneCollection*> childSet;
            for (const SceneCollection* child : collection->children)
            {
                if (!Contains(child) || child->parent != collection.get() ||
                    !childSet.insert(child).second)
                    return fail("collection child relationship is invalid");
            }

            if (collection.get() != masterCollection)
            {
                if (!Contains(collection->parent) ||
                    PointerCount(
                        collection->parent->children,
                        collection.get()) != 1)
                    return fail("collection parent relationship is invalid");
            }

            std::unordered_set<const SceneCollection*> ancestry;
            const SceneCollection* root = nullptr;
            for (const SceneCollection* current = collection.get();
                 current != nullptr;
                 current = current->parent)
            {
                if (!Contains(current) || !ancestry.insert(current).second)
                    return fail("collection hierarchy contains a cycle");
                root = current;
            }
            if (root != masterCollection)
                return fail("collection is detached from the master collection");

            std::unordered_set<const Node*> linkedNodes;
            for (const Node* node : collection->nodes)
            {
                if (!Contains(node) || !linkedNodes.insert(node).second)
                    return fail("collection node link is invalid or duplicated");
            }
        }

        if (error != nullptr)
            error->clear();
        return true;
    }

private:
    template<typename T>
    [[nodiscard]] static bool HasPointer(
        const std::vector<T*>& values,
        const T* target) noexcept
    {
        return std::find(values.begin(), values.end(), target) != values.end();
    }

    template<typename T>
    [[nodiscard]] static std::size_t PointerCount(
        const std::vector<T*>& values,
        const T* target) noexcept
    {
        return static_cast<std::size_t>(
            std::count(values.begin(), values.end(), target));
    }

    template<typename T>
    static bool ErasePointer(std::vector<T*>& values, const T* target)
    {
        const auto found = std::find(values.begin(), values.end(), target);
        if (found == values.end())
            return false;
        values.erase(found);
        return true;
    }

    [[nodiscard]] bool WouldCreateNodeCycle(
        const Node* node,
        const Node* parent) const noexcept
    {
        for (const Node* current = parent;
             current != nullptr;
             current = current->parent)
        {
            if (current == node)
                return true;
        }
        return false;
    }

    [[nodiscard]] bool WouldCreateCollectionCycle(
        const SceneCollection* collection,
        const SceneCollection* parent) const noexcept
    {
        for (const SceneCollection* current = parent;
             current != nullptr;
             current = current->parent)
        {
            if (current == collection)
                return true;
        }
        return false;
    }

    std::vector<std::unique_ptr<Node>> nodes;
    std::vector<std::unique_ptr<SceneCollection>> collections;
    SceneCollection* masterCollection = nullptr;
    SceneCollection* activeCollection = nullptr;
};
}
