# Vortex3D Master Roadmap

# Foundation Lock Requirement

## STATUS: FOUNDATION LOCKED (v0.0.4)

Foundation systems must be hardened before feature expansion.

---

# Milestone 0 — Foundation Lock

## Geometry Foundation

- [x] Half-edge groundwork
- [x] Half-edge mesh kernel
- [x] Safe invalid index handling
- [x] Twin edge support
- [x] Edge linking support
- [x] Topology validation
- [x] Reciprocal twin and endpoint validation
- [x] Closed, face-consistent loop validation
- [x] Vertex/face representative validation
- [x] Invalid topology regression tests

Higher-level modeling operations:
- [ ] Edge split
- [ ] Face split
- [ ] Extrusion
- [ ] Bevel
- [ ] Subdivision

These are modeling features, not kernel blockers.

---

## Completed Foundation Systems

- [x] Initial project structure
- [x] Import/export interfaces
- [x] UUID foundation
- [x] Object foundation
- [x] Result foundation
- [x] Resource handle foundation
- [x] Matrix4 foundation
- [x] Vector operators
- [x] Half-edge kernel foundation
- [x] Mesh validation foundation
- [x] Unified `Vortex` namespace
- [x] Scene/import/export type compatibility
- [x] UUID collision hardening
- [x] Move-only `Result<T>` support
- [x] CTest foundation regression suite

---

## Scene Foundation

- [x] UUID-backed scene nodes
- [x] Automatic master scene collection
- [x] Nested scene collections
- [x] Multi-collection node linking
- [x] Active collection tracking
- [x] Collection visibility, selectability, and renderability
- [x] Parent-state propagation for collection flags
- [x] Duplicate-link protection
- [x] Node and collection cycle prevention
- [x] Cross-scene reference protection
- [x] Safe node unlink and deletion behavior
- [x] Content-preserving collection deletion
- [x] Scene hierarchy and ownership validation
- [x] Scene collection regression tests

---

# Next Milestone

## Asset Foundation

Required next:
- Asset manager
- Virtual filesystem
- Resource cache
- Asset loading pipeline

Do not add editor tools, renderer systems, AI systems, or advanced importers until foundation milestones are complete.
