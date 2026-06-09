
# Contributing to FakeSignal

> *"Why would anyone contribute to a library that intentionally crashes their program?"*
> 
> – A reasonable person

First of all: **thank you for considering contributing to FakeSignal.**  
Second of all: **are you okay?** 🤡

---

## Table of Contents

1. [Code of Conduct](#code-of-conduct)
2. [What We Accept](#what-we-accept)
3. [What We DON'T Accept](#what-we-dont-accept)
4. [Adding a New Fake Signal](#adding-a-new-fake-signal)
5. [Code Style](#code-style)
6. [Testing](#testing)
7. [Pull Request Process](#pull-request-process)
8. [FAQ](#faq)

---

## Code of Conduct

**TL;DR:** Don't be a jerk. This is a joke library. Keep it fun.

**Full version:**

- Be respectful – even when someone suggests adding a signal that deletes `/etc`
- Accept that your PR might be rejected because the crash isn't "creative enough"
- Laugh at the absurdity of this project
- Don't open serious legal issues about undefined behavior – we know

**Violations of the CoC will result in:**
1. A gentle reminder that this is a joke
2. A slightly less gentle reminder
3. We'll `raise("SIGSEGV")` in your honor

---

## What We Accept

| Type | Accepted? | Notes |
|------|-----------|-------|
| New fake signals | ✅ Yes | Must be **real crashes**, not exceptions |
| Cross-platform fixes | ✅ Yes | Linux, Windows, macOS |
| Documentation improvements | ✅ Yes | More humor welcome |
| Example programs | ✅ Yes | The more dramatic, the better |
| CMake improvements | ✅ Yes | Professional trolls use build systems |
| Bug fixes | ⚠️ Depends | Only if the bug is "library doesn't crash when it should" |
| Unit tests | ✅ Yes | Tests that **expect to crash** are golden |

---

## What We DON'T Accept

| Type | Reason |
|------|--------|
| **Making it safe** | That defeats the entire purpose |
| **Removing crashes** | Literally why does this library exist then? |
| **Adding exception-based signals** | Java is that way → ☕ |
| **Signal handlers that "fix" the crash** | Not possible. Undefined behavior is undefined. |
| **Porting to Rust** | Rust doesn't have undefined behavior (well, almost). Also, why? |
| **Serious bug reports saying "it crashed"** | Read the README. That's the feature. |
| **Requesting support for production use** | Seek professional help |

---

## Adding a New Fake Signal

### Step 1: Choose a creative name

We support any string, but classics are:
- `SIGSEGV`, `SIGFPE`, `SIGABRT`, `SIGILL`
- Creative ones: `INFINITE_LOOP`, `MEMORY_LEAK`, `DEADLOCK`, `STACK_OVERFLOW`

Proposal for new ones:
- `SIGDANGLING` – use-after-free
- `SIGDOUBLEFREE` – double deletion
- `SIGRACECOND` – data race (hard to reproduce, bonus points if nondeterministic)
- `SIGCORRUPT` – buffer overflow on the stack

### Step 2: Implement the crash

**Template for a new signal:**

```cpp
else if (sig == "YOUR_SIGNAL_NAME") {
    detail::announceCrash(sig);  // optional but fun
    // YOUR CRASH IMPLEMENTATION HERE
    // Must be:
    // - Real (segfault, hang, leak, UB)
    // - Not catchable (no exceptions)
    // - Impressive to watch
}
```

**Examples of good crashes:**

| Signal | Implementation | Why it's good |
|--------|----------------|----------------|
| `SIGSEGV` | `int* p = nullptr; *p = 42;` | Classic, reliable, hardware-verified |
| `STACK_OVERFLOW` | `raise(sig);` | Elegant recursion |
| `MEMORY_LEAK` | `while(true) new int[1000000];` | Slow, painful death |
| `DEADLOCK` | Two mutexes, two threads | Hangs forever, looks like progress |

### Step 3: Update documentation

Add your signal to:
- `README.md` – the table of supported signals
- This file (`CONTRIBUTING.md`) – as an example

### Step 4: Add a test (optional but recommended)

```cpp
// In tests/test_runner.cpp or a new test file
TEST(FakeSignal, YOUR_SIGNAL_CRASHES) {
    EXPECT_DEATH(fakesignal::raise("YOUR_SIGNAL"), ".*");
}
```

### Step 5: Submit a PR

---

## Code Style

We have one rule: **make it crash.**

But if you care about aesthetics:

### Indentation
- Use spaces (2 or 4, we don't fight about tabs vs spaces here)
- We're too busy crashing to format code perfectly

### Naming
- `snake_case` for functions and variables (C++-style)
- `UPPER_CASE` for signal names (as strings)

### Comments
- Funny comments are encouraged
- Serious comments are tolerated
- No comments means you trust the reader to understand UB (brave)

**Example of a good comment:**
```cpp
else if (sig == "SIGFPE") {
    int x = 1 / 0;  // This may be optimized away.
                    // If your compiler is "smart", try volatile int y = 1; y / 0;
}
```

---

## Testing

### Philosophy
**A test passes if it crashes.**  
**A test fails if it doesn't crash.**  
**A test that runs to completion is a bug.**

### Running tests

```bash
cd build
cmake .. -DFAKESIGNAL_BUILD_TESTS=ON
make
ctest
```

Expected output:
```
100% tests passed, 0 tests failed out of 6
```

…even though every test crashed. That's the magic of `WILL_FAIL` in CMake.

### Writing a test for a new signal

```cpp
// In tests/test_signal_name.cpp
#include "fakesignal.h"

int main() {
    fakesignal::raise("YOUR_SIGNAL");
    return 0;  // Never reached
}
```

Add to `CMakeLists.txt`:
```cmake
add_crash_test(YOUR_SIGNAL "YOUR_SIGNAL" 139)
```

---

## Pull Request Process

1. **Fork the repo** (and question your life choices)
2. **Create a branch** (`git checkout -b feature/new-signal-hang`)
3. **Implement your crash** (make it glorious)
4. **Update documentation** (README.md + this file)
5. **Run tests** (they should crash – that's good)
6. **Commit with a funny message**:
   ```
   git commit -m "✨ Add SIGDANGLING (use-after-free simulation)"
   ```
7. **Push and open a PR**

### PR Title Format
```
[FEATURE] Add SIG<NAME>
[FIX] Make SIG<EXISTING> crash harder
[DOC] Improve humor level in README
[BUILD] CMake improvements for <platform>
```

### PR Description Template

```markdown
## What does this PR do?
Adds support for fake signal: `SIG_XXXXX`

## How does it crash?
[Brief explanation of the undefined behavior used]

## Platform support
- [x] Linux
- [x] Windows  
- [x] macOS

## Testing
- [ ] Test passes (crashes as expected)
- [ ] No other signals broken

## Additional notes
[Any funny remarks or warnings]
```

### Review Process

| Role | Responsibility |
|------|----------------|
| You | Write crashing code |
| Maintainer | Laugh, then merge (or reject with a joke) |
| CI | Crash (if it doesn't crash, CI fails – correctly) |

**Expected turnaround time:** Between 1 hour and never (we're busy crashing our own programs)

---

## FAQ

### Q: Why would I contribute to this?
**A:** Good question. Some possibilities:
- You have too much free time
- You want to learn how undefined behavior works (by experimenting)
- You think it's funny
- You lost a bet

### Q: My PR makes the library safe. Can you merge it?
**A:** No. Go away. 👋

### Q: Can I add a signal that formats the hard drive?
**A:** No. That's malicious, not funny. We crash programs, not computers.

### Q: Can I add a signal that sends SIGSEGV to another process?
**A:** That's called `kill()`, not FakeSignal. But interesting idea… open an issue for discussion.

### Q: Will you accept a Rust version?
**A:** Only if it uses `unsafe` and still crashes. But then why not just use C++?

### Q: My company wants to use this in production. What should I do?
**A:** Update your resume. You'll need it.

### Q: This library broke my CI/CD pipeline!
**A:** Working as intended. Remove it from your pipeline.

### Q: I have a serious bug report.
**A:** If it's about the library **not** crashing when it should, that's a real bug. Please open an issue with a reproducible example.

---

## Recognition

Contributors who add especially creative crashes will be:
- Added to the `CONTRIBUTORS.md` file
- Given a virtual high five ✋
- Mentioned in the next release notes (with a joke)

**Hall of Fame (to be created):**
- First person to add a signal that works across all 3 platforms
- Most creative undefined behavior
- Best commit message

---

## Final Note

Remember: **This is a joke library.**  
Don't take it seriously.  
Don't use it in production.  
Don't show it to your manager.

But if you laugh while reading this, we've succeeded. 😄

---

**Happy crashing! 💀**

*– The FakeSignal Maintainer (who really should be working on something else)*
```

---

## Bonus: File `CONTRIBUTORS.md` (để thêm sau)

```markdown
# Contributors to FakeSignal

## Core Team
- **Your Name** – Original idea, professional procrastinator

## Hall of Fame 🏆
*No contributors yet. Be the first to add a creative crash!*

## How to get on this list
1. Submit a PR that adds a new crash
2. Have it merged
3. Accept your virtual high five ✋

---

*Last updated: Never (because nobody has contributed yet)*
