
# 🧨 FakeSignal

[![C++](https://img.shields.io/badge/C++-17-blue.svg)](https://isocpp.org/)
[![License](https://img.shields.io/badge/License-WTFPL-brightgreen.svg)](http://www.wtfpl.net/)
[![Status](https://img.shields.io/badge/Status-WONTFIX-red.svg)]()
[![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20Windows%20%7C%20macOS-lightgrey.svg)]()
[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg)](CONTRIBUTING.md)

[Tiếng Việt](VN.md) | English
> **"Fake in name, real in nature – just like my will to live after debugging this."**

## 📦 What is FakeSignal?

**FakeSignal** is a C++ library that provides a `raise()` function for **fake signals**… that **really crash your program** using real undefined behavior.

Yes, you read that right.  
It's a library for developers who want to simulate signals *the hard way* — by actually segfaulting, dividing by zero, overflowing the stack, or leaking memory until the OOM killer shows up.

### Why?

Because sometimes you want to test:
- Your signal handler (will it survive a real crash? spoiler: no ❌)
- Your debugging skills 🐛
- Your will to continue programming 😭

Also: `std::raise(SIGSEGV)` is too boring. We prefer **real, hardware-verified crashes**.

---

## 🚀 Quick Start

### 1. Include the header

```cpp
#include "fakesignal.h"
```

### 2. Call raise() with your favorite fake signal

```cpp
int main() {
    fakesignal::raise("SIGSEGV");
    // You will never reach this line.
    // Unless you're in a parallel universe where nullptr dereference is legal.
    return 0;
}
```

### 3. Compile (remember to include fakesignal.cpp)

```bash
g++ -std=c++17 -pthread main.cpp fakesignal.cpp -o demo
./demo
Segmentation fault (core dumped)  # ✅ Working as intended
```

That's it. Your program now crashes on demand.

---

## 🧨 Supported "Signals"

| Signal | Implementation | Real Crash? | Humor Level |
|--------|----------------|-------------|--------------|
| `SIGSEGV` | `int* p = nullptr; *p = 42;` | ✅ Yes | 💀💀💀 |
| `SIGFPE` | `int x = 1 / 0;` | ✅ Yes* | 💀💀 |
| `SIGABRT` | `std::abort();` | ✅ Yes | 💀 |
| `SIGILL` | `asm volatile("ud2");` | ✅ Yes (x86) | 💀💀💀 |
| `INFINITE_LOOP` | `while(true) {}` | ⚠️ No crash, just hang | 💀 |
| `STACK_OVERFLOW` | Recursive `raise()` | ✅ Yes | 💀💀💀💀 |
| `MEMORY_LEAK` | `while(true) new int[1000000];` | ✅ Eventually (OOM) | 💀💀 |
| `DEADLOCK` | Two mutexes, two threads | ⚠️ Hang forever | 💀💀💀 |

*\*May be optimized away by aggressive compilers – we call that "feature, not bug"*

---

## 📖 Full Examples

### Example 1: Basic crash

```cpp
#include "fakesignal.h"

int main() {
    fakesignal::raise("SIGSEGV");  // 💥
}
```

### Example 2: Interactive signal selector

```cpp
#include <iostream>
#include "fakesignal.h"

int main() {
    std::string sig;
    std::cout << "Enter signal (SIGSEGV, SIGFPE, INFINITE_LOOP): ";
    std::cin >> sig;
    fakesignal::raise(sig);  // 💥 or 🔄
}
```

### Example 3: Try to catch the uncatchable

```cpp
#include <csignal>
#include <iostream>
#include "fakesignal.h"

void handler(int) {
    std::cout << "Caught! But can I recover? 😅" << std::endl;
}

int main() {
    signal(SIGSEGV, handler);
    fakesignal::raise("SIGSEGV");
    // Output: Caught! But can I recover?
    // Then: Segmentation fault (core dumped)
    // Spoiler: you can't recover from UB
}
```

### Example 4: Raise all signals (crashes on first)

```cpp
#include "fakesignal.h"

int main() {
    fakesignal::raiseAll();  // 💥💥💥
}
```

### Example 5: Dry run (no crash, boring but exists)

```cpp
#include "fakesignal.h"

int main() {
    fakesignal::dryRun("SIGSEGV");
    std::cout << "Program continues... (unlike the real version)" << std::endl;
}
```

---

## 📁 Project Structure

```
fakesignal/
├── README.md
├── CONTRIBUTING.md
├── LICENSE
├── fakesignal.h          ← Header (declarations)
├── fakesignal.cpp        ← Implementation (the real crashes)
└── examples/
    ├── crash_demo.cpp
    ├── signal_selector.cpp
    ├── crash_all.cpp
    └── dry_run_demo.cpp
```

---

## 🛠 Installation

### Option 1: Copy-paste (recommended for quick pain)

Just copy `fakesignal.h` and `fakesignal.cpp` into your project and compile them together.

```bash
g++ -std=c++17 -pthread your_code.cpp fakesignal.cpp -o your_program
```

### Option 2: Static library

```bash
g++ -std=c++17 -c fakesignal.cpp -o fakesignal.o
ar rcs libfakesignal.a fakesignal.o
g++ -std=c++17 your_code.cpp -L. -lfakesignal -pthread -o your_program
```

### Option 3: CMake (for masochists)

```bash
git clone https://github.com/dryfish09/fakesignal.git
cd fakesignal
mkdir build && cd build
cmake .. -DFAKESIGNAL_BUILD_EXAMPLES=ON
make
sudo make install  # if you dare
```

Then in your `CMakeLists.txt`:

```cmake
find_package(fakesignal REQUIRED)
target_link_libraries(your_target fakesignal)
```

---

## 🧪 Building and Running Examples

```bash
git clone https://github.com/yourusername/fakesignal.git
cd fakesignal

# Compile an example manually
g++ -std=c++17 -pthread examples/crash_demo.cpp fakesignal.cpp -o crash_demo
./crash_demo

# Or use CMake
mkdir build && cd build
cmake .. -DFAKESIGNAL_BUILD_EXAMPLES=ON
make
./examples/crash_demo
./examples/signal_selector
./examples/crash_all
```

---

## ⚠️ Important Warnings

| Warning | Explanation |
|---------|-------------|
| **This library is NOT fake** | The crashes are 100% real. Your program WILL terminate abnormally. |
| **DO NOT use in production** | Unless you hate your job and want to be featured on [The Daily WTF](https://thedailywtf.com). |
| **DO NOT open issues saying "my program crashes"** | That's the entire point. Read the README. |
| **Signal handlers won't save you** | Undefined behavior is, by definition, undefined. You can't recover from dereferencing nullptr. |
| **May cause data loss** | If you haven't saved your work, now would be a good time. |

### What happens when you catch SIGSEGV?

```cpp
void handler(int) {
    // You are here. But the instruction that faulted?
    // It's not retrying. You can't just "fix" nullptr.
    // longjmp? UB. Modify the instruction pointer? Good luck.
    std::cout << "I'm in danger 🫠" << std::endl;
}
```

The only portable thing to do in a SIGSEGV handler is:
- Log the error (maybe)
- Flush logs
- `_exit(128 + SIGSEGV)`

---

## 🧪 Testing

FakeSignal comes with tests that **expect to crash**. That's how we know they pass.

```bash
cd build
cmake .. -DFAKESIGNAL_BUILD_TESTS=ON
make
ctest
```

Expected output:
```
100% tests passed, 0 tests failed out of 4
```
…even though every test crashed. That's the magic of `WILL_FAIL`.

---

## 🤝 Contributing

We welcome contributions… but why?

See [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

**Ideas for new "signals":**
- `SIGDANGLING` – use-after-free
- `SIGDOUBLEFREE` – double deletion
- `SIGRACECOND` – data race
- `SIGFORMAT` – string format exploit simulation

Open a PR if you're brave (or foolish).

---

## 📜 License

**WTFPL** – Do What The Fuck You Want To Public License.

In plain English:
- ✅ Use it anywhere
- ✅ Modify it however you like
- ✅ Burn it, print it, fold it into a paper plane
- ❌ Don't sue me if it crashes (it will)
- ❌ Don't ask me to fix it (I can't fix undefined behavior)

[![License: WTFPL](https://img.shields.io/badge/License-WTFPL-brightgreen.svg)](http://www.wtfpl.net/)

*"With great power comes great segfault."*

---

## 🙏 Acknowledgments

- **nullptr** – for always being there when we need to crash
- **Undefined Behavior** – for never letting us down
- **The C++ Standard Committee** – for making this possible
- **My therapist** – for listening to me explain why I wrote this

---

## ⭐ Star History

If you star this repo, your code will still crash.  
But at least you'll have a shiny star. ⭐

---

## 📞 Support

**There is no support.**  
This is a library that intentionally crashes your program.  
What kind of support did you expect? 😅

For existential support, contact your local philosopher.

---

## 📋 Requirements

- C++17 or later
- A computer that can segfault (all of them)
- A sense of humor 🤡
- No will to live (optional)
- `pthread` (for `DEADLOCK` and threading features)

### Platform Support

| Platform | Compiler | Status |
|----------|----------|--------|
| Linux | GCC 7+, Clang 5+ | ✅ Fully supported |
| Windows | MSVC 2017+, MinGW | ✅ Supported |
| macOS | Apple Clang | ✅ Supported |
| Embedded | Any C++17 compiler | ⚠️ May brick your device (not our fault) |

---

## 🐛 Known Issues

| Issue | Status | Workaround |
|-------|--------|-------------|
| `SIGFPE` may be optimized away | WONTFIX | Use `volatile` or `-O0` |
| `SIGILL` doesn't work on ARM | WONTFIX | Buy an x86 CPU |
| Deadlock never returns (obviously) | WONTFIX | That's what deadlock means |
| People keep opening crash reports | WONTFIX | Read the README |

---

## 🔮 Future Plans

- [ ] Add `SIGDANGLING` (use-after-free)
- [ ] Add `SIGCORRUPT` (heap buffer overflow)
- [ ] Add `SIGRACE` (data race with sanitizers)
- [ ] Write a blog post titled "Why I Wrote a Library That Crashes Your Program"
- [ ] Get featured on r/programminghorror
- [ ] Regret everything

---

**Made with ❤️ and 💀 by someone who should have used Rust.**

---

*Last words before segfault: "It works on my machine."*
