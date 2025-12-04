# Mini Cashier

## Cara Build & Run

### ### **Linux (bash)**

```bash

g++ -std=c++20 -Wall -Wextra -Wpedantic -g $(find src -name '*.cpp') \
    -o build/Debug/outDebug

./build/Debug/outDebug
```

---

### ### **Windows (PowerShell)**

```powershell

g++ -std=c++20 -Wall -Wextra -Wpedantic -g (Get-ChildItem -Recurse src\*.cpp) \
    -o build\Debug\outDebug.exe

./build/Debug/outDebug.exe
```

---
