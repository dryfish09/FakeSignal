
# 🧨 FakeSignal

[![C++](https://img.shields.io/badge/C++-17-blue.svg)](https://isocpp.org/)
[![License](https://img.shields.io/badge/License-WTFPL-brightgreen.svg)](http://www.wtfpl.net/)
[![Status](https://img.shields.io/badge/Status-WONTFIX-red.svg)]()
[![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20Windows%20%7C%20macOS-lightgrey.svg)]()
[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg)](CONTRIBUTING.md)

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

```cpp
#include "fakesignal.h"

int main() {
    fakesignal::raise("SIGSEGV");
    // You will never reach this line.
    // Unless you're in a parallel universe where nullptr dereference is legal.
    return 0;
}
```

Compile and run:
```bash
g++ -std=c++17 -pthread main.cpp -o demo
./demo
Segmentation fault (core dumped)  # ✅ Working as intended
```

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

## 📖 Examples

### Basic crash
```cpp
#include "fakesignal.h"

int main() {
    fakesignal::raise("SIGSEGV");  // 💥
}
```

### Interactive signal selector
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

### Try to catch the uncatchable
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

---

## 🛠 Requirements

- C++17 or later
- A computer that can segfault (all of them)
- A sense of humor 🤡
- No will to live (optional)

### Platform Support
| Platform | Status |
|----------|--------|
| Linux (GCC/Clang) | ✅ Fully supported |
| Windows (MSVC) | ✅ Supported (with `/unsafe` where needed) |
| macOS (Clang) | ✅ Supported |
| Embedded systems | ⚠️ May brick your device (not our fault) |

---

## 📥 Installation

### Option 1: Copy-paste (recommended for quick pain)
Just copy `fakesignal.h` into your project and include it.

### Option 2: CMake (for masochists)
```bash
git clone https://github.com/yourusername/fakesignal.git
cd fakesignal
mkdir build && cd build
cmake .. -DFAKESIGNAL_BUILD_EXAMPLES=ON
make
sudo make install  # if you dare
```

### Option 3: Package managers (we wish)
Not yet. But if you want to maintain Homebrew or vcpkg ports for this, please seek professional help first.

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

**Made with ❤️ and 💀 by someone who should have used Rust.**

---

*Last words before segfault: "It works on my machine."*
