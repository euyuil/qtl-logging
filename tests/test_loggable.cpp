// Copyright (c) 2025 Liu Yue. All rights reserved.

#include <qtl/logging.h>

#include <gtest/gtest.h>

using namespace qtl::logging;

class TestLoggable : public Loggable {
public:
    using Loggable::Loggable;

    void logSomething() {
        QTL_INFO("Test message");
    }
};

TEST(LoggableTest, DirectConstruction) {
    // Test direct construction with explicit sink managers (advanced API)
    auto params = std::make_shared<LoggingParams>(LoggingParams::consoleOnly());
    auto consoleSink = std::make_shared<ConsoleSinkManager>(params);
    auto fileSink = std::make_shared<FileSinkManager>(params);

    EXPECT_NO_THROW({
        TestLoggable loggable(params, consoleSink, fileSink, "TestLogger");
        loggable.logSomething();
    });
}

TEST(LoggableTest, RegistryConstruction) {
    // Test registry construction with simplified API
    LoggerRegistry::initialize(std::make_shared<LoggingParams>(LoggingParams::consoleOnly()));

    EXPECT_NO_THROW({
        TestLoggable loggable("TestLogger2");
        loggable.logSomething();
    });
}

TEST(LoggableTest, MultipleMacros) {
    // Test all logging macros with simplified API
    LoggerRegistry::initialize(
        std::make_shared<LoggingParams>(LoggingParams::consoleOnly().withConsoleLevel("trace")));

    class MacroTest : public Loggable {
    public:
        MacroTest() : Loggable("MacroTest") {}

        void testAllLevels() {
            QTL_TRACE("Trace message");
            QTL_DEBUG("Debug message");
            QTL_INFO("Info message");
            QTL_WARN("Warn message");
            QTL_ERROR("Error message");
            QTL_CRITICAL("Critical message");
        }
    };

    EXPECT_NO_THROW({
        MacroTest test;
        test.testAllLevels();
    });
}
