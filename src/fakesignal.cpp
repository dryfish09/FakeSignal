// fakesignal.cpp
// Implementation of the legendary FakeSignal library.
// Because putting everything in the header is too mainstream.
//
// WARNING: This file intentionally contains undefined behavior.
// Do NOT use in production. Do NOT run without saving your work.

#include "fakesignal.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <cstdlib>

#ifdef __linux__
#include <sys/mman.h>
#endif

#ifdef _WIN32
#include <windows.h>
#include <intrin.h>   // for __ud2
#endif

namespace fakesignal {

    // Helper: print a dramatic goodbye message (optional)
    namespace detail {
        void announceCrash(const std::string& sig) {
            // Use cerr because it's unbuffered — we won't lose the message on crash
            std::cerr << "[FakeSignal] Raising fake signal: " << sig
                      << " (this is real crash, not fake)\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }

    // Public API – guaranteed to crash or hang as advertised
    void raise(const std::string& sig) {
        if (sig == "SIGSEGV") {
            detail::announceCrash(sig);
            volatile int* p = nullptr;   // volatile: prevent compiler from removing
            *p = 42;                     // BOOM
        }
        else if (sig == "SIGFPE") {
            detail::announceCrash(sig);
            volatile int zero = 0;       // volatile: prevent constant folding
            volatile int x = 1 / zero;   // BOOM
            (void)x;
        }
        else if (sig == "SIGABRT") {
            detail::announceCrash(sig);
            std::abort();
        }
        else if (sig == "SIGDANGLING") {
            detail::announceCrash(sig);
            int* p = new int(42);        // allocate
            delete p;                    // free
            *p = 10;                     // use-after-free 💥
        }
        else if (sig == "SIGDOUBLEFREE") {
            detail::announceCrash(sig);
            int* p = new int(42);
            delete p;
            delete p;                    // double free 💥
        }
        else if (sig == "SIGILL") {
            detail::announceCrash(sig);
#if defined(__x86_64__) || defined(__i386__)
            asm volatile("ud2");         // illegal instruction (x86)
#elif defined(_MSC_VER)
            __ud2();                     // MSVC intrinsic
#else
            std::abort();                // fallback (ARM, etc.)
#endif
        }
        else if (sig == "INFINITE_LOOP") {
            detail::announceCrash(sig);
            while (true) {
                std::this_thread::yield();   // CPU goes brrr (politely)
            }
        }
        else if (sig == "STACK_OVERFLOW") {
            detail::announceCrash(sig);
            fakesignal::raise(sig);      // recursion, no base case
        }
        else if (sig == "STACK_OVERFLOW_SOFT") {
            detail::announceCrash(sig);
            volatile char large[1000000];   // 1 MB on stack
            (void)large;
            fakesignal::raise("STACK_OVERFLOW_SOFT");
        }
        else if (sig == "MEMORY_LEAK") {
            detail::announceCrash(sig);
            while (true) {
                // Leak ~10 MB per iteration until OOM killer shows up
                int* leak = new int[2'500'000];
                (void)leak;
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }
        else if (sig == "DEADLOCK") {
            detail::announceCrash(sig);
            static std::mutex m1, m2;
            std::thread t1([&]() {
                std::lock_guard<std::mutex> lock1(m1);
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                std::lock_guard<std::mutex> lock2(m2);
            });
            std::thread t2([&]() {
                std::lock_guard<std::mutex> lock2(m2);
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                std::lock_guard<std::mutex> lock1(m1);
            });
            t1.join();
            t2.join();   // Never reached
        }
        else {
            std::cerr << "[FakeSignal] Unknown signal: " << sig << ". Aborting.\n";
            std::abort();
        }
    }

    // Bonus: raise multiple signals in sequence (for masochists)
    void raiseAll() {
        const char* signals[] = {
            "SIGSEGV", "SIGFPE", "SIGABRT", "SIGILL",
            "INFINITE_LOOP", "STACK_OVERFLOW", "MEMORY_LEAK"
        };
        for (const char* sig : signals) {
            // Use cerr + endl to flush immediately — we might not survive the next line
            std::cerr << "Raising " << sig << "..." << std::endl;
            raise(sig);
            std::cerr << "You won't see this." << std::endl;
        }
    }

    // Safe version – just prints, no crash (boring, but exists for completeness)
    void dryRun(const std::string& sig) {
        std::cout << "[DryRun] Would have crashed with: " << sig << "\n";
        std::cout << "But you're lucky – this is just a simulation.\n";
    }

} // namespace fakesignal
