#include "fakesignal.h"

int main() {
    // Expect to crash – that's the test passing for FakeSignal
    fakesignal::raise(TEST_SIGNAL);
    return 0;
}
