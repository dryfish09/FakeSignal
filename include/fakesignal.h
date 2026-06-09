// fakesignal.h
#ifndef FAKESIGNAL_H
#define FAKESIGNAL_H

#include <string>
#include <thread>
#include <mutex>

namespace fakesignal {

    // Main API – raises a fake signal that crashes for real
    void raise(const std::string& sig);

    // Raises all known signals in sequence (guaranteed to crash on first)
    void raiseAll();

    // Dry run – no crash, just prints what would happen
    void dryRun(const std::string& sig);

} // namespace fakesignal

#endif // FAKESIGNAL_H
