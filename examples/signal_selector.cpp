#include <iostream>
#include "fakesignal.h"

int main() {
    std::cout << "Enter signal name (SIGSEGV, SIGFPE, INFINITE_LOOP, etc.): ";
    std::string sig;
    std::cin >> sig;
    fakesignal::raise(sig);
    return 0;
}
