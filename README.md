# learn-cpp

Build and run with CMake:

```bash
mkdir -p build
cmake -S . -B build
cmake --build build -- -j$(nproc)
./build/learn-cpp
```

Project layout:

- [CMakeLists.txt](CMakeLists.txt) — top-level CMake configuration
- `src/` — source files (contains `main.cpp`)
- `include/` — public headers
- `examples/` — small example programs (built as `example-<name>`)
- `exercises/` — scratch files for experiments (built as `exercise-<name>`)

Build and run examples:

```bash
cmake -S . -B build
cmake --build build -- -j$(nproc)
./build/example-hello
```
