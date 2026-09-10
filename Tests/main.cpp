#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <type_traits>

#include "Core/EngineVersion.h"
#include "Core/Object.h"
#include "Core/ResourceHandle.h"
#include "Core/Result.h"
#include "Core/UUID.h"
#include "Geometry/HalfEdgeMesh.h"
#include "Geometry/MeshValidation.h"
#include "IO/Exporter.h"
#include "IO/Importer.h"
#include "Material/Material.h"
#include "Math/Matrix4.h"
#include "Scene/Scene.h"

namespace
{
int failures = 0;

void Check(bool condition, const char* message)
{
    if (!condition)
    {
        std::cerr << "[FAIL] " << message << '\n';
        ++failures;
    }
}

Vortex::HalfEdgeMesh MakeTriangle()
{
    Vortex::HalfEdgeMesh mesh;
    const auto v0 = mesh.AddVertex();
    const auto v1 = mesh.AddVertex();
    const auto v2 = mesh.AddVertex();
    const auto face = mesh.AddFace();
    const auto e0 = mesh.AddEdge(v0);
    const auto e1 = mesh.AddEdge(v1);
    const auto e2 = mesh.AddEdge(v2);

    Check(mesh.SetNext(e0, e1), "triangle next edge 0");
    Check(mesh.SetNext(e1, e2), "triangle next edge 1");
    Check(mesh.SetNext(e2, e0), "triangle next edge 2");
    Check(mesh.SetFace(e0, face), "triangle face edge 0");
    Check(mesh.SetFace(e1, face), "triangle face edge 1");
    Check(mesh.SetFace(e2, face), "triangle face edge 2");
    return mesh;
}

void TestHalfEdge()
{
    auto triangle = MakeTriangle();
    std::string error;
    Check(triangle.ValidateTopology(&error), "valid triangle topology");
    Check(error.empty(), "successful topology validation clears error");

    Vortex::HalfEdgeMesh invalid;
    Check(invalid.AddEdge(0) == Vortex::InvalidIndex,
          "edge rejects missing vertex");
    const auto vertex = invalid.AddVertex();
    const auto edge0 = invalid.AddEdge(vertex);
    const auto edge1 = invalid.AddEdge(vertex);
    const auto edge2 = invalid.AddEdge(vertex);
    Check(!invalid.SetTwin(edge0, edge0), "self twin is rejected");
    Check(!invalid.SetNext(edge0, edge0), "self next is rejected");
    Check(invalid.SetTwin(edge0, edge1), "valid twin is accepted");
    Check(invalid.SetTwin(edge0, edge2), "twin reassignment succeeds");
    Check(!invalid.Edges()[edge1].HasTwin(), "old twin link is cleared");
    Check(invalid.Edges()[edge0].twin == edge2 &&
          invalid.Edges()[edge2].twin == edge0,
          "new twin relationship is reciprocal");

    Vortex::HalfEdgeMesh openFace;
    const auto openVertex = openFace.AddVertex();
    const auto openFaceIndex = openFace.AddFace();
    const auto openEdge = openFace.AddEdge(openVertex);
    Check(openFace.SetFace(openEdge, openFaceIndex), "assign open face");
    Check(!openFace.ValidateTopology(), "open face is rejected");

    Vortex::HalfEdgeMesh mixedFace;
    const auto mv0 = mixedFace.AddVertex();
    const auto mv1 = mixedFace.AddVertex();
    const auto mv2 = mixedFace.AddVertex();
    const auto mf0 = mixedFace.AddFace();
    const auto mf1 = mixedFace.AddFace();
    const auto me0 = mixedFace.AddEdge(mv0);
    const auto me1 = mixedFace.AddEdge(mv1);
    const auto me2 = mixedFace.AddEdge(mv2);
    Check(mixedFace.SetNext(me0, me1), "mixed face next 0");
    Check(mixedFace.SetNext(me1, me2), "mixed face next 1");
    Check(mixedFace.SetNext(me2, me0), "mixed face next 2");
    Check(mixedFace.SetFace(me0, mf0), "mixed face assignment 0");
    Check(mixedFace.SetFace(me1, mf1), "mixed face assignment 1");
    Check(mixedFace.SetFace(me2, mf0), "mixed face assignment 2");
    Check(!mixedFace.ValidateTopology(), "cross-face loop is rejected");

    Vortex::HalfEdgeMesh repeatedVertex;
    const auto rv0 = repeatedVertex.AddVertex();
    const auto rv1 = repeatedVertex.AddVertex();
    const auto rf = repeatedVertex.AddFace();
    const auto re0 = repeatedVertex.AddEdge(rv0);
    const auto re1 = repeatedVertex.AddEdge(rv1);
    const auto re2 = repeatedVertex.AddEdge(rv1);
    Check(repeatedVertex.SetNext(re0, re1) &&
          repeatedVertex.SetNext(re1, re2) &&
          repeatedVertex.SetNext(re2, re0), "repeated vertex face loop");
    Check(repeatedVertex.SetFace(re0, rf) &&
          repeatedVertex.SetFace(re1, rf) &&
          repeatedVertex.SetFace(re2, rf), "repeated vertex face assignment");
    Check(!repeatedVertex.ValidateTopology(), "repeated face vertex is rejected");

    Vortex::HalfEdgeMesh shared;
    const auto sv0 = shared.AddVertex();
    const auto sv1 = shared.AddVertex();
    const auto sv2 = shared.AddVertex();
    const auto sv3 = shared.AddVertex();
    const auto sf0 = shared.AddFace();
    const auto sf1 = shared.AddFace();
    const auto sa0 = shared.AddEdge(sv0);
    const auto sa1 = shared.AddEdge(sv1);
    const auto sa2 = shared.AddEdge(sv2);
    const auto sb0 = shared.AddEdge(sv1);
    const auto sb1 = shared.AddEdge(sv0);
    const auto sb2 = shared.AddEdge(sv3);
    Check(shared.SetNext(sa0, sa1) && shared.SetNext(sa1, sa2) &&
          shared.SetNext(sa2, sa0), "first shared-edge face loop");
    Check(shared.SetNext(sb0, sb1) && shared.SetNext(sb1, sb2) &&
          shared.SetNext(sb2, sb0), "second shared-edge face loop");
    Check(shared.SetFace(sa0, sf0) && shared.SetFace(sa1, sf0) &&
          shared.SetFace(sa2, sf0), "first shared-edge face assignment");
    Check(shared.SetFace(sb0, sf1) && shared.SetFace(sb1, sf1) &&
          shared.SetFace(sb2, sf1), "second shared-edge face assignment");
    Check(shared.SetTwin(sa0, sb0), "shared edge twin assignment");
    Check(shared.ValidateTopology(&error), "valid reversed shared edge");

    Check(shared.ClearTwin(sa0), "clear valid shared twin");
    Check(shared.SetTwin(sa0, sb1), "assign mismatched shared twin");
    Check(!shared.ValidateTopology(), "non-opposite twin endpoints are rejected");

    Vortex::HalfEdgeMesh boundary;
    const auto bv0 = boundary.AddVertex();
    const auto bv1 = boundary.AddVertex();
    const auto be0 = boundary.AddEdge(bv0);
    const auto be1 = boundary.AddEdge(bv1);
    Check(boundary.SetNext(be0, be1) && boundary.SetNext(be1, be0),
          "boundary edge loop");
    Check(boundary.SetTwin(be0, be1), "boundary twin assignment");
    Check(boundary.ValidateTopology(), "valid two-half-edge boundary loop");
}

void TestMeshValidation()
{
    Vortex::Mesh mesh;
    mesh.vertices = {
        {{0.0F, 0.0F, 0.0F}, {}},
        {{1.0F, 0.0F, 0.0F}, {}},
        {{0.0F, 1.0F, 0.0F}, {}}
    };
    mesh.edges = {{0, 1}, {1, 2}, {2, 0}};
    mesh.faces = {{{0, 1, 2}}};
    Check(Vortex::MeshValidation::Valid(mesh), "valid mesh");

    mesh.faces[0].vertices[2] = 99;
    Check(!Vortex::MeshValidation::Valid(mesh), "face index bounds");
    mesh.faces[0].vertices[2] = 2;
    mesh.edges[0] = {0, 0};
    Check(!Vortex::MeshValidation::Valid(mesh), "degenerate edge");
    mesh.edges[0] = {0, 1};
    mesh.vertices[0].position.x = std::numeric_limits<float>::infinity();
    Check(!Vortex::MeshValidation::Valid(mesh), "non-finite geometry");
    mesh.vertices[0].position.x = 0.0F;
    mesh.edges.push_back({1, 0});
    Check(!Vortex::MeshValidation::Valid(mesh), "duplicate undirected edge");
}

void TestCore()
{
    const Vortex::UUID explicitId(10'000);
    const Vortex::UUID generated;
    Check(explicitId.Value() == 10'000, "explicit UUID value");
    Check(generated.Value() > explicitId.Value(), "generated UUID avoids collision");

    auto moveOnly = Vortex::Result<std::unique_ptr<int>>::Success(
        std::make_unique<int>(42));
    Check(moveOnly.IsSuccess() && *moveOnly.Value() == 42,
          "Result supports move-only values");

    auto failure = Vortex::Result<int>::Failure("expected failure");
    Check(!failure.IsSuccess() && failure.Error() == "expected failure",
          "Result failure retains error");

    Vortex::ResourceHandle<int> empty;
    Vortex::ResourceHandle<int> valid(7);
    Check(!empty && valid && valid.ID() == 7, "resource handle validity");

    const auto identity = Vortex::Matrix4::Identity();
    Check(identity.data[0] == 1.0F && identity.data[15] == 1.0F,
          "identity matrix");
}

void TestScene()
{
    static_assert(!std::is_copy_constructible_v<Vortex::Node>);
    static_assert(!std::is_copy_constructible_v<Vortex::SceneCollection>);
    static_assert(!std::is_move_constructible_v<Vortex::Scene>);

    Vortex::Scene scene;
    Vortex::Scene other;
    auto* master = scene.MasterCollection();
    Check(master != nullptr && scene.ActiveCollection() == master,
          "scene creates active master collection");
    Check(master->Name() == "Scene Collection" && master->Parent() == nullptr,
          "master collection identity");
    Check(scene.Collections().size() == 1,
          "new scene owns exactly one collection");

    auto* root = scene.CreateNode("root");
    auto* child = scene.CreateNode("child", root);
    auto* foreign = other.CreateNode("foreign");
    Check(root != nullptr && child != nullptr, "scene creates nodes");
    Check(child->Parent() == root && root->Children().size() == 1,
          "scene establishes hierarchy");
    Check(scene.IsLinked(master, root) && scene.IsLinked(master, child),
          "new nodes link to the active collection");
    Check(scene.CreateNode("invalid", foreign) == nullptr,
          "scene rejects foreign parent");

    auto* environment = scene.CreateCollection("Environment");
    auto* props = scene.CreateCollection("Props", environment);
    auto* characters = scene.CreateCollection("Characters");
    Check(environment != nullptr && props != nullptr && characters != nullptr,
          "scene creates nested collections");
    Check(environment->Parent() == master && props->Parent() == environment,
          "collection hierarchy is established");
    Check(!scene.CreateCollection("Foreign", other.MasterCollection()),
          "scene rejects foreign collection parent");

    Check(scene.SetActiveCollection(props), "set active collection");
    auto* propNode = scene.CreateNode("Crate");
    Check(propNode != nullptr && scene.IsLinked(props, propNode),
          "active collection receives new nodes");
    Check(scene.LinkNode(characters, propNode),
          "one node can link to multiple collections");
    Check(!scene.LinkNode(characters, propNode),
          "duplicate collection link is rejected");
    Check(scene.CollectionsFor(propNode).size() == 2,
          "node collection memberships are discoverable");
    Check(scene.FindNode(propNode->GetID()) == propNode &&
          scene.FindCollection(props->GetID()) == props,
          "scene objects are found by UUID");

    environment->SetVisible(false);
    Check(!props->IsEffectivelyVisible() && !props->IsEffectivelySelectable(),
          "parent visibility propagates to child collection");
    environment->SetVisible(true);
    environment->SetSelectable(false);
    Check(!props->IsEffectivelySelectable(),
          "parent selectability propagates to child collection");
    environment->SetSelectable(true);
    environment->SetRenderable(false);
    Check(!props->IsEffectivelyRenderable(),
          "parent renderability propagates to child collection");
    environment->SetRenderable(true);
    props->SetName("Set Dressing");
    Check(props->Name() == "Set Dressing", "collection can be renamed");

    Check(!scene.ReparentCollection(environment, props),
          "collection cycle is rejected");
    Check(!scene.ReparentCollection(master, props),
          "master collection cannot be reparented");
    Check(scene.ReparentCollection(props, master) &&
          scene.ReparentCollection(props, environment),
          "collection can be safely reparented");
    Check(!scene.SetActiveCollection(other.MasterCollection()),
          "foreign active collection is rejected");
    Check(!scene.LinkNode(other.MasterCollection(), root) &&
          !scene.LinkNode(master, foreign),
          "cross-scene links are rejected");

    Check(!scene.SetNodeParent(root, child), "node parent cycle is rejected");
    Check(scene.SetNodeParent(child, nullptr) && child->Parent() == nullptr,
          "node can be detached from transform parent");
    Check(scene.SetNodeParent(child, root), "node can be safely reparented");

    auto* temporary = scene.CreateCollection("Temporary");
    auto* nested = scene.CreateCollection("Nested", temporary);
    Check(scene.SetActiveCollection(temporary),
          "temporary collection can become active");
    auto* preserved = scene.CreateNode("Preserved");
    const auto temporaryId = temporary->GetID();
    Check(scene.DeleteCollection(temporary), "collection can be deleted");
    Check(scene.FindCollection(temporaryId) == nullptr &&
          scene.ActiveCollection() == master,
          "deleting active collection restores active parent");
    Check(nested->Parent() == master && scene.IsLinked(master, preserved),
          "deleting collection preserves children and node contents");
    Check(!scene.DeleteCollection(master), "master collection cannot be deleted");

    Check(scene.LinkNode(characters, root),
          "node can be linked before deletion");
    const auto rootId = root->GetID();
    Check(scene.DeleteNode(root), "node can be deleted");
    Check(scene.FindNode(rootId) == nullptr && child->Parent() == nullptr,
          "node deletion detaches transform children");
    Check(scene.CollectionsFor(child).size() == 1,
          "deleting parent does not delete child node");

    Check(scene.UnlinkNode(characters, propNode), "node can be unlinked");
    Check(!scene.UnlinkNode(characters, propNode),
          "missing collection link is reported");
    Check(scene.Contains(propNode), "unlinking does not delete node");

    std::string error;
    Check(scene.Validate(&error), "scene collection topology validates");
    Check(error.empty(), "successful scene validation clears error");
}
}

int main()
{
    TestCore();
    TestHalfEdge();
    TestMeshValidation();
    TestScene();

    if (failures != 0)
    {
        std::cerr << failures << " foundation test(s) failed\n";
        return 1;
    }

    std::cout << "Vortex3D "
              << Vortex::EngineVersion::Major << '.'
              << Vortex::EngineVersion::Minor << '.'
              << Vortex::EngineVersion::Patch
              << ": all foundation tests passed\n";
    return 0;
}
