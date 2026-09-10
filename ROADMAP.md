# Vortex3D Master Roadmap

# Foundation Lock Requirement

## STATUS: BLOCKING

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

---

# Next Milestone

## Asset Foundation

Required next:
- Asset manager
- Virtual filesystem
- Resource cache
- Asset loading pipeline

Do not add editor tools, renderer systems, AI systems, or advanced importers until foundation milestones are complete.
