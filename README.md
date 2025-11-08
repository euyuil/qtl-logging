# qtl-logging

A lightweight, standalone C++20 logging library extracted from the trading-engine project. Built on top of [spdlog](https://github.com/gabime/spdlog), it provides a clean, dependency-free interface for logging with support for console and file outputs, log rotation, and flexible configuration.

## Features

- **Lightweight Design**: No heavy dependencies on DI frameworks or configuration parsers
- **Flexible Configuration**: Simple data structure with builder pattern for easy setup
- **Console & File Logging**: Support for both console and file outputs with independent log levels
- **Log Rotation**: Built-in support for size-based and daily log rotation
- **Mixin Pattern**: Easy integration via `Loggable` base class
- **Global Registry**: Optional singleton pattern for accessing loggers across your application
- **Thread-Safe**: Multi-threaded sinks for concurrent logging
- **Modern C++20**: Clean, type-safe API

## Requirements

- C++20 compiler (GCC 10+, Clang 12+, MSVC 2019+)
- CMake 3.15+
- spdlog 1.12.0+
- Google Test 1.14.0+ (for tests)

## Quick Start

### Install Dependencies

qtl-logging requires spdlog to be installed on your system.

**Ubuntu/Debian:**
```bash
sudo apt install libspdlog-dev libgtest-dev
```

**macOS (Homebrew):**
```bash
brew install spdlog googletest
```

**Other systems (vcpkg):**
```bash
vcpkg install spdlog gtest
cmake -DCMAKE_TOOLCHAIN_FILE=[vcpkg]/scripts/buildsystems/vcpkg.cmake ..
```

### Build and Install

```bash
git clone https://github.com/yourusername/qtl-logging.git
cd qtl-logging
mkdir build && cd build
cmake ..
cmake --build .
sudo cmake --install .
```

### Basic Usage

#### Using the Global Registry

```cpp
#include <qtl/logging/logger_registry.h>
#include <qtl/logging/logging_params.h>
#include <qtl/logging/console_sink_manager.h>
#include <qtl/logging/file_sink_manager.h>

using namespace qtl::logging;

int main() {
    // Configure logging
    auto params = std::make_shared<LoggingParams>(
        LoggingParams::defaults()
            .withConsoleLevel("info")
            .withFileOutput(true)
            .withFilePrefix("myapp")
    );

    auto consoleSink = std::make_shared<ConsoleSinkManager>(params);
    auto fileSink = std::make_shared<FileSinkManager>(params);

    // Initialize global registry
    LoggerRegistry::initialize(params, consoleSink, fileSink);

    // Get loggers by name
    auto logger = LoggerRegistry::getLogger("MyComponent");
    logger->info("Application started");
    logger->warn("This is a warning");

    return 0;
}
```

#### Using the Loggable Mixin

```cpp
#include <qtl/logging/loggable.h>
#include <qtl/logging/logger_registry.h>

using namespace qtl::logging;

class MyComponent : public Loggable {
public:
    MyComponent() : Loggable("MyComponent") {
        QTL_INFO("MyComponent created");
    }

    void doSomething(int value) {
        QTL_DEBUG("Processing value: {}", value);

        if (value < 0) {
            QTL_ERROR("Invalid value: {}", value);
            return;
        }

        QTL_INFO("Successfully processed: {}", value);
    }
};

int main() {
    // Initialize logging (same as above)
    auto params = std::make_shared<LoggingParams>();
    auto consoleSink = std::make_shared<ConsoleSinkManager>(params);
    auto fileSink = std::make_shared<FileSinkManager>(params);
    LoggerRegistry::initialize(params, consoleSink, fileSink);

    // Use the component
    MyComponent component;
    component.doSomething(42);

    return 0;
}
```

## Configuration

### LoggingParams

The `LoggingParams` struct provides all configuration options:

```cpp
LoggingParams params;

// Console settings
params.logToConsole = true;
params.consoleLogLevel = "info";  // trace, debug, info, warn, error, critical
params.consoleLogPattern = "[%Y-%m-%d %H:%M:%S.%f] [%t] [%^%L%$] [%n] %v";

// File settings
params.logToFile = true;
params.logFilePrefix = "myapp";
params.fileLogLevel = "trace";
params.fileLogPattern = "[%Y-%m-%d %H:%M:%S.%f] [%P] [%t] [%L] [%n] %v";
params.fileFlushOn = "info";

// Rotation settings
params.enableRotation = true;
params.maxFileSize = 10 * 1024 * 1024;  // 10MB
params.maxFiles = 10;
params.rotateDaily = false;

// Overall logger level
params.logLevel = "trace";
```

### Builder Pattern

Use the fluent builder API for cleaner configuration:

```cpp
auto params = LoggingParams::defaults()
    .withConsoleOutput(true)
    .withConsoleLevel("info")
    .withFileOutput(true)
    .withFilePrefix("myapp")
    .withFileLevel("debug")
    .withRotation(true, 10, 5)  // 10MB, 5 files
    .withDailyRotation(false);
```

### Factory Methods

```cpp
// Console-only logging
auto params1 = LoggingParams::consoleOnly();

// File-only logging
auto params2 = LoggingParams::fileOnly("myapp");

// Default configuration
auto params3 = LoggingParams::defaults();
```

## Available Logging Macros

When using the `Loggable` mixin, the following macros are available:

```cpp
QTL_TRACE("Trace message: {}", value);
QTL_DEBUG("Debug message: {}", value);
QTL_INFO("Info message: {}", value);
QTL_WARN("Warning message: {}", value);
QTL_ERROR("Error message: {}", value);
QTL_CRITICAL("Critical message: {}", value);
```

These macros use [fmt](https://github.com/fmtlib/fmt) syntax for formatting (bundled with spdlog).

## Log Rotation

### Size-Based Rotation

```cpp
auto params = LoggingParams::defaults()
    .withFileOutput(true)
    .withFilePrefix("myapp")
    .withRotation(true, 10, 5);  // 10MB per file, keep 5 files
```

This creates files like: `myapp.log`, `myapp.1.log`, `myapp.2.log`, etc.

### Daily Rotation

```cpp
auto params = LoggingParams::defaults()
    .withFileOutput(true)
    .withFilePrefix("myapp")
    .withDailyRotation(true);
```

This creates files like: `myapp_2025-11-08.log`, `myapp_2025-11-09.log`, etc.

## CMake Integration

### Using as a Subdirectory

```cmake
add_subdirectory(qtl-logging)
target_link_libraries(your_target PRIVATE qtl::qtl-logging)
```

### Using find_package

After installation:

```cmake
find_package(qtl-logging REQUIRED)
target_link_libraries(your_target PRIVATE qtl::qtl-logging)
```

## Project Structure

```
qtl-logging/
├── include/
│   └── qtl/
│       ├── logging/
│       │   ├── console_sink_manager.h
│       │   ├── file_sink_manager.h
│       │   ├── level.h
│       │   ├── loggable.h
│       │   ├── logger_registry.h
│       │   └── logging_params.h
│       └── util/
│           └── string_hash.h
├── src/
│   ├── console_sink_manager.cpp
│   ├── file_sink_manager.cpp
│   ├── loggable.cpp
│   ├── logger_registry.cpp
│   └── logging_params.cpp
├── tests/
│   ├── test_loggable.cpp
│   ├── test_logger_registry.cpp
│   ├── test_logging_params.cpp
│   └── test_sink_managers.cpp
├── examples/
│   ├── loggable_example.cpp
│   └── simple_example.cpp
└── CMakeLists.txt
```

## Building and Testing

```bash
# Configure
mkdir build && cd build
cmake ..

# Build
cmake --build .

# Run tests
ctest --output-on-failure

# Run examples
./examples/simple_example
./examples/loggable_example
```

### CMake Options

- `QTL_LOGGING_BUILD_TESTS` (default: ON) - Build unit tests
- `QTL_LOGGING_BUILD_EXAMPLES` (default: ON) - Build examples

## License

This library is part of the qtl project. See LICENSE file for details.

## Contributing

Contributions are welcome! Please ensure:
- Code follows C++20 standards
- All tests pass
- New features include tests
- Code is properly documented

## Credits

Built on top of [spdlog](https://github.com/gabime/spdlog) by Gabi Melman.
