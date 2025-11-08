// Copyright (c) 2025 Liu Yue. All rights reserved.

#include <qtl/logging.h>

#include <memory>

using namespace qtl::logging;

class MyComponent : public Loggable {
public:
    MyComponent() : Loggable("MyComponent") {
        QTL_INFO("MyComponent created");
    }

    void doWork() {
        QTL_DEBUG("Starting work...");
        QTL_INFO("Processing item: {}", 123);
        QTL_DEBUG("Work completed");
    }
};

class AnotherComponent : public Loggable {
public:
    AnotherComponent() : Loggable("AnotherComponent") {
        QTL_INFO("AnotherComponent initialized");
    }

    void process(int value) {
        QTL_TRACE("Entering process with value: {}", value);
        QTL_INFO("Processing value: {}", value * 2);
        QTL_TRACE("Exiting process");
    }
};

int main() {
    // Initialize logging (simplified API)
    LoggerRegistry::initialize(
        std::make_shared<LoggingParams>(
            LoggingParams::defaults()
                .withConsoleLevel("debug")
                .withFileLevel("trace")
                .withFileOutput(true)
                .withFilePrefix("loggable_example")));

    // Create components (they automatically get loggers)
    MyComponent comp1;
    comp1.doWork();

    AnotherComponent comp2;
    comp2.process(42);

    return 0;
}
