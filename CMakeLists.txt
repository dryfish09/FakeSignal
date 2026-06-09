# CMakeLists.txt
# FakeSignal – The library that crashes on purpose

cmake_minimum_required(VERSION 3.16)
project(fakesignal VERSION 1.0.0 DESCRIPTION "Fake signals that cause real crashes" LANGUAGES CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build options
option(FAKESIGNAL_BUILD_EXAMPLES "Build example programs" ON)
option(FAKESIGNAL_BUILD_TESTS "Build unit tests (they will crash)" OFF)
option(FAKESIGNAL_ENABLE_ASAN "Enable AddressSanitizer for debugging" OFF)
option(FAKESIGNAL_ENABLE_WARNINGS "Enable all warnings" ON)

# Warning flags
if(FAKESIGNAL_ENABLE_WARNINGS)
    if(MSVC)
        add_compile_options(/W4 /WX-)
    else()
        add_compile_options(-Wall -Wextra -Wpedantic -Wshadow -Wconversion)
    endif()
endif()

# Sanitizer support (for when you want to see the crash in slow motion)
if(FAKESIGNAL_ENABLE_ASAN)
    if(MSVC)
        message(WARNING "ASAN not fully supported on MSVC in this setup")
    else()
        add_compile_options(-fsanitize=address -g)
        add_link_options(-fsanitize=address)
    endif()
endif()

# ============================================================================
# Main library
# ============================================================================
add_library(fakesignal STATIC
    src/fakesignal.cpp
)

target_include_directories(fakesignal
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Platform-specific definitions
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_definitions(fakesignal PRIVATE _CRT_SECURE_NO_WARNINGS)
    # MSVC needs /arch:AVX for some inline assembly (optional)
    if(FAKESIGNAL_ENABLE_ASM)
        target_compile_options(fakesignal PRIVATE /arch:AVX)
    endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(fakesignal PRIVATE -g)
endif()

# Threads for DEADLOCK and co.
find_package(Threads REQUIRED)
target_link_libraries(fakesignal Threads::Threads)

# Set properties
set_target_properties(fakesignal PROPERTIES
    PUBLIC_HEADER "include/fakesignal.h"
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
)

# ============================================================================
# Install rules
# ============================================================================
include(GNUInstallDirs)

install(TARGETS fakesignal
    EXPORT fakesignalTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT fakesignalTargets
    FILE fakesignalTargets.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/fakesignal
)

# ============================================================================
# Examples
# ============================================================================
if(FAKESIGNAL_BUILD_EXAMPLES)
    add_executable(crash_demo examples/crash_demo.cpp)
    target_link_libraries(crash_demo fakesignal)
    target_include_directories(crash_demo PRIVATE include)

    add_executable(signal_selector examples/signal_selector.cpp)
    target_link_libraries(signal_selector fakesignal)
    target_include_directories(signal_selector PRIVATE include)

    # The "full experience" – raises all signals (will crash on first)
    add_executable(crash_all examples/crash_all.cpp)
    target_link_libraries(crash_all fakesignal)
    target_include_directories(crash_all PRIVATE include)

    if(FAKESIGNAL_ENABLE_ASAN)
        message(STATUS "ASAN enabled – crash traces will be beautiful")
    endif()
endif()

# ============================================================================
# Tests (they are expected to crash – that's the point)
# ============================================================================
if(FAKESIGNAL_BUILD_TESTS)
    enable_testing()
    
    # Each test is expected to fail/crash – that's success for FakeSignal
    function(add_crash_test NAME SIGNAL EXPECTED_RETURN_CODE)
        add_executable(test_${NAME} tests/test_runner.cpp)
        target_compile_definitions(test_${NAME} PRIVATE TEST_SIGNAL="${SIGNAL}")
        target_link_libraries(test_${NAME} fakesignal)
        target_include_directories(test_${NAME} PRIVATE include)
        add_test(NAME FakeSignal_${NAME} COMMAND test_${NAME})
        set_tests_properties(FakeSignal_${NAME} PROPERTIES 
            WILL_FAIL ON  # Because crashing is expected
            PASS_REGULAR_EXPRESSION "FakeSignal"
        )
    endfunction()

    add_crash_test(SEGV "SIGSEGV" 139)
    add_crash_test(FPE "SIGFPE" 136)
    add_crash_test(ABRT "SIGABRT" 134)
    add_crash_test(ILL "SIGILL" 132)
    # INFINITE_LOOP and DEADLOCK are skipped – they never return
    message(STATUS "Note: INFINITE_LOOP, DEADLOCK, and STACK_OVERFLOW tests are skipped (they never exit)")
endif()

# ============================================================================
# Helper: Print configuration summary
# ============================================================================
message(STATUS "")
message(STATUS "=== FakeSignal Configuration ===")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build examples: ${FAKESIGNAL_BUILD_EXAMPLES}")
message(STATUS "Build tests: ${FAKESIGNAL_BUILD_TESTS}")
message(STATUS "AddressSanitizer: ${FAKESIGNAL_ENABLE_ASAN}")
message(STATUS "=================================")
message(STATUS "")
