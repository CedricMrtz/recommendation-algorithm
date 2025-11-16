# recommendation-algorithm

## Build Instructions

### Requirements
- CMake 3.16+
- C++17 compiler
  - **Windows:** Qt 6.x + MinGW (installed in `C:/Qt/6.x.x/mingw_64`)
  - **Linux:** Qt6 development packages (`qt6-base-dev`)
  - **macOS:** Qt 6.x via Homebrew (`brew install qt`)

---

## Build
```bash
git clone <repo>
cd recommendation-algorithm
mkdir build
cd build
cmake ..
cmake --build .
```
---
## Run
./front        # Linux / macOS
./front.exe    # Windows
