#include "fakesignal.h"

int main() {
    fakesignal::raiseAll();  // Crashes on first signal
    return 0;
}
