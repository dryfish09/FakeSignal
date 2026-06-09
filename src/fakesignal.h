#ifndef FAKESIGNAL_H
#define FAKESIGNAL_H

#include <string>
#include <cstdlib>

#ifdef __linux__
#include <sys/mman.h>
#endif

namespace fakesignal {

    inline void raise(const std::string& sig) {
        if (sig == "SIGSEGV") {
            int* p = nullptr;
            *p = 42;  // Segmentation fault (real)
        }
        else if (sig == "SIGFPE") {
            int x = 1 / 0;  // Divide by zero
        }
        else if (sig == "SIGABRT") {
            std::abort();
        }
        else if (sig == "SIGILL") {
            #ifdef __x86_64__
            asm volatile("ud2");  // Illegal instruction
            #else
            std::abort();  // fallback
            #endif
        }
        else if (sig == "INFINITE_LOOP") {
            while (true) {}
        }
        else if (sig == "STACK_OVERFLOW") {
            raise(sig);  // Recursion until death
        }
        else if (sig == "MEMORY_LEAK") {
            while (true) {
                new int[1000000];  // Leak until OOM
            }
        }
    }

} // namespace fakesignal

#endif // FAKESIGNAL_H
