#include <qtl/logging/console_sink_manager.h>
#include <qtl/logging/file_sink_manager.h>
#include <qtl/logging/loggable.h>
#include <qtl/logging/logger_registry.h>
#include <qtl/logging/logging_params.h>

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
    auto params = std::make_shared<LoggingParams>(LoggingParams::consoleOnly());
    auto consoleSink = std::make_shared<ConsoleSinkManager>(params);
    auto fileSink = std::make_shared<FileSinkManager>(params);

    EXPECT_NO_THROW({
        TestLoggable loggable(params, consoleSink, fileSink, "TestLogger");
        loggable.logSomething();
    });
}

TEST(LoggableTest, RegistryConstruction) {
    auto params = std::make_shared<LoggingParams>(LoggingParams::consoleOnly());
    auto consoleSink = std::make_shared<ConsoleSinkManager>(params);
    auto fileSink = std::make_shared<FileSinkManager>(params);

    LoggerRegistry::initialize(params, consoleSink, fileSink);

    EXPECT_NO_THROW({
        TestLoggable loggable("TestLogger2");
        loggable.logSomething();
    });
}

TEST(LoggableTest, MultipleMacros) {
    auto params = std::make_shared<LoggingParams>(LoggingParams::consoleOnly().withConsoleLevel("trace"));
    auto consoleSink = std::make_shared<ConsoleSinkManager>(params);
    auto fileSink = std::make_shared<FileSinkManager>(params);

    LoggerRegistry::initialize(params, consoleSink, fileSink);

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
