// fakesignal.cpp
// Implementation of the legendary FakeSignal library.
// Because putting everything in the header is too mainstream.

#include "fakesignal.h"
#include <iostream>
#include <thread>
#include <chrono>

#ifdef __linux__
#include <sys/mman.h>
#endif

#ifdef _WIN32
#include <windows.h>
#endif

namespace fakesignal {

    // Helper: print a dramatic goodbye message (optional)
    namespace detail {
        void announceCrash(const std::string& sig) {
            std::cerr << "[FakeSignal] Raising fake signal: " << sig
                      << " (this is real crash, not fake)\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }

    // Public API – guaranteed to crash or hang as advertised
    void raise(const std::string& sig) {
        if (sig == "SIGSEGV") {
            detail::announceCrash(sig);
            int* p = nullptr;
            *p = 42;  // BOOM
        }
        else if (sig == "SIGFPE") {
            detail::announceCrash(sig);
            int x = 1 / 0;  // BOOM (if compiler doesn't optimize)
        }
        else if (sig == "SIGABRT") {
            detail::announceCrash(sig);
            std::abort();
        }
        else if (sig == "SIGILL") {
            detail::announceCrash(sig);
#ifdef __x86_64__
            asm volatile("ud2");  // illegal instruction
#elif defined(_WIN32)
            __ud2();  // MSVC intrinsic
#else
            std::abort();
#endif
        }
        else if (sig == "INFINITE_LOOP") {
            detail::announceCrash(sig);
            while (true) {
                // Busy wait – CPU goes brrr
                std::this_thread::yield();
            }
        }
        else if (sig == "STACK_OVERFLOW") {
            detail::announceCrash(sig);
            raise(sig);  // recursion – infinite, no base case
        }
        else if (sig == "MEMORY_LEAK") {
            detail::announceCrash(sig);
            while (true) {
                // Leak 10 MB per iteration until system cries
                int* leak = new int[2'500'000];  // ~10 MB on 32-bit int
                (void)leak;  // Prevent "unused variable" warning
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
            t2.join();  // Never reached
        }
        else if (sig == "STACK_OVERFLOW_SOFT") {
            detail::announceCrash(sig);
            char large[1000000];  // 1 MB on stack – might overflow, might not
            (void)large;
            raise("STACK_OVERFLOW_SOFT");
        }
        else {
            // Unknown signal – fallback to abort
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
            std::cout << "Raising " << sig << "...\n";
            raise(sig);
            std::cout << "You won't see this.\n";
        }
    }

    // Safe version – just prints, no crash (boring, but exists for completeness)
    void dryRun(const std::string& sig) {
        std::cout << "[DryRun] Would have crashed with: " << sig << "\n";
        std::cout << "But you're lucky – this is just a simulation.\n";
    }

} // namespace fakesignal
