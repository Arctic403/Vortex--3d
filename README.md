# Vortex3D Core v0.0.4

Foundation layer for Vortex3D.

Goals:
- Universal scene representation
- Import/export architecture
- Editable geometry foundation
- UUID-backed scene graph and collection organization
- Renderer-independent core

Scene organization:
- Every scene owns an automatic master collection.
- Collections nest independently of transform parenting.
- Nodes can link to multiple collections without duplicated ownership.
- Collection deletion preserves linked nodes and reparents child collections.

Build and test:

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```
