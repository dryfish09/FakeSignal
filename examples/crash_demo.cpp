#include <iostream>
#include "fakesignal.h"

int main() {
    std::cout << "Raising SIGSEGV in 2 seconds...\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));
    fakesignal::raise("SIGSEGV");
    return 0;
}
