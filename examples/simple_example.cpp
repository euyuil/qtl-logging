// Copyright (c) 2025 Liu Yue. All rights reserved.

#include <qtl/logging.h>

#include <memory>

using namespace qtl::logging;

int main() {
    // Initialize logging (simplified API)
    LoggerRegistry::initialize(
        std::make_shared<LoggingParams>(
            LoggingParams::defaults()
                .withConsoleOutput(true)
                .withConsoleLevel("debug")   // Set console to debug level
                .withLogLevel("debug")       // Set overall log level to debug
                .withFileOutput(true)
                .withFilePrefix("simple_example")
                .withRotation(true, 5, 3)),  // 5MB, 3 files
        "logs");                             // Session directory

    // Get loggers
    auto logger1 = LoggerRegistry::getLogger("Component1");
    auto logger2 = LoggerRegistry::getLogger("Component2");

    // Log messages
    logger1->trace("This is a trace message");
    logger1->debug("This is a debug message");
    logger1->info("This is an info message");
    logger1->warn("This is a warning message");
    logger1->error("This is an error message");

    logger2->info("Component2 initialized");
    logger2->info("Processing data: {}", 42);

    // Flush all loggers
    LoggerRegistry::flushAll();

    return 0;
}
