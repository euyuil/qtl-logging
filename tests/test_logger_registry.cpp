#include <qtl/logging.h>

#include <gtest/gtest.h>

using namespace qtl::logging;

class LoggerRegistryTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Each test gets fresh registry state
        auto params = std::make_shared<LoggingParams>(LoggingParams::consoleOnly());
        auto consoleSink = std::make_shared<ConsoleSinkManager>(params);
        auto fileSink = std::make_shared<FileSinkManager>(params);

        LoggerRegistry::initialize(params, consoleSink, fileSink);
    }
};

TEST_F(LoggerRegistryTest, IsInitialized) {
    EXPECT_TRUE(LoggerRegistry::isInitialized());
}

TEST_F(LoggerRegistryTest, GetLogger) {
    auto logger = LoggerRegistry::getLogger("TestLogger");
    EXPECT_NE(logger, nullptr);
    EXPECT_EQ(logger->name(), "TestLogger");
}

TEST_F(LoggerRegistryTest, GetSameLoggerTwice) {
    auto logger1 = LoggerRegistry::getLogger("SameLogger");
    auto logger2 = LoggerRegistry::getLogger("SameLogger");
    EXPECT_EQ(logger1, logger2);
}

TEST_F(LoggerRegistryTest, GetMultipleLoggers) {
    auto logger1 = LoggerRegistry::getLogger("Logger1");
    auto logger2 = LoggerRegistry::getLogger("Logger2");
    EXPECT_NE(logger1, logger2);
    EXPECT_EQ(logger1->name(), "Logger1");
    EXPECT_EQ(logger2->name(), "Logger2");
}

TEST_F(LoggerRegistryTest, GetDefaultLogger) {
    auto logger = LoggerRegistry::getDefaultLogger();
    EXPECT_NE(logger, nullptr);
}

TEST_F(LoggerRegistryTest, FlushAll) {
    auto logger = LoggerRegistry::getLogger("FlushTest");
    logger->info("Test message");

    EXPECT_NO_THROW(LoggerRegistry::flushAll());
}

TEST_F(LoggerRegistryTest, GetParams) {
    auto params = LoggerRegistry::getParams();
    EXPECT_NE(params, nullptr);
    EXPECT_TRUE(params->logToConsole);
}

TEST_F(LoggerRegistryTest, GetSinkManagers) {
    auto consoleSink = LoggerRegistry::getConsoleSinkManager();
    auto fileSink = LoggerRegistry::getFileSinkManager();

    EXPECT_NE(consoleSink, nullptr);
    EXPECT_NE(fileSink, nullptr);
}
