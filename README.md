# Vortex3D Core v0.0.3

Foundation layer for Vortex3D.

Goals:
- Universal scene representation
- Import/export architecture
- Editable geometry foundation
- Renderer-independent core

Build and test:

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```
