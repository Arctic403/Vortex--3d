# Vortex3D Master Roadmap

# Foundation Lock Requirement

## STATUS: BLOCKING

No new major features should be added until the Foundation Lock checklist is completed.

This prevents architectural debt and ensures editor, rendering, import/export, and AI systems are built on stable foundations.

---

# Milestone 0 — Foundation Lock

## Required Before Feature Expansion

## Build System
- [ ] Separate engine library from tests
- [ ] Prepare editor linkage architecture
- [ ] Add platform-ready CMake structure

## Math Foundation
- [x] Matrix4 foundation
- [x] Vector math operators
- [ ] Matrix operations
- [ ] Quaternion operations
- [ ] Ray support
- [ ] Plane support
- [ ] Bounding boxes / AABB

## Core Systems
- [x] UUID system
- [ ] Persistent UUID support
- [ ] Object naming
- [ ] Serialization support
- [x] Result/error foundation
- [ ] Result<void> support
- [x] Resource handle foundation

## Ownership and Memory
- [ ] Finalize ownership rules
- [ ] Resource lifetime rules
- [ ] Memory profiling rules

## Scene Foundation
- [x] Scene foundation
- [ ] Stable hierarchy ownership
- [ ] Scene serialization preparation

## Geometry Foundation
- [x] Half-edge groundwork
- [ ] Complete half-edge mesh kernel
- [x] Mesh validation foundation

Required before modeling tools:
- Vertex operations
- Edge operations
- Face operations
- Extrusion support
- Bevel support
- Subdivision support

## Asset Foundation
- [ ] Asset manager
- [ ] Virtual filesystem
- [ ] Resource cache
- [ ] Asset loading pipeline

## Serialization
- [ ] Scene serialization
- [ ] Material serialization
- [ ] Prefab serialization

## Import / Export
After foundations are complete:

Order:
1. OBJ
2. glTF
3. FBX

Required:
- Error handling
- Import options
- Export options
- Progress reporting

## Build Pipeline
- [ ] GitHub Actions
- [ ] Windows build
- [ ] Linux build
- [ ] Android preparation
- [ ] Automated tests
- [ ] Build caching

---

# Current Completed Foundation Work

- [x] Roadmap tracking
- [x] Initial project structure
- [x] Importer/Exporter interface
- [x] UUID foundation
- [x] Base Object system
- [x] Quaternion foundation
- [x] Scene foundation
- [x] Engine version system
- [x] Logging foundation
- [x] Result foundation
- [x] Smart ownership foundation
- [x] Matrix4 foundation
- [x] Vector math operators
- [x] Half-edge groundwork
- [x] Mesh validation foundation
- [x] Resource handle foundation

---

# Rule Going Forward

Foundation first.

Do not add:
- Editor tools
- Rendering systems
- AI systems
- Plugin systems
- Advanced importers

until the Foundation Lock checklist is complete.
