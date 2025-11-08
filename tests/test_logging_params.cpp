#include <qtl/logging/logging_params.h>

#include <gtest/gtest.h>

using namespace qtl::logging;

TEST(LoggingParamsTest, Defaults) {
    auto params = LoggingParams::defaults();
    EXPECT_TRUE(params.logToConsole);
    EXPECT_FALSE(params.logToFile);
    EXPECT_EQ(params.consoleLogLevel, "info");
    EXPECT_EQ(params.logFilePrefix, "app");
}

TEST(LoggingParamsTest, ConsoleOnly) {
    auto params = LoggingParams::consoleOnly();
    EXPECT_TRUE(params.logToConsole);
    EXPECT_FALSE(params.logToFile);
}

TEST(LoggingParamsTest, FileOnly) {
    auto params = LoggingParams::fileOnly("test");
    EXPECT_FALSE(params.logToConsole);
    EXPECT_TRUE(params.logToFile);
    EXPECT_EQ(params.logFilePrefix, "test");
}

TEST(LoggingParamsTest, BuilderPattern) {
    auto params = LoggingParams::defaults()
                      .withConsoleOutput(false)
                      .withFileOutput(true)
                      .withFilePrefix("my_app")
                      .withConsoleLevel("debug")
                      .withFileLevel("trace")
                      .withRotation(true, 20, 5);

    EXPECT_FALSE(params.logToConsole);
    EXPECT_TRUE(params.logToFile);
    EXPECT_EQ(params.logFilePrefix, "my_app");
    EXPECT_EQ(params.consoleLogLevel, "debug");
    EXPECT_EQ(params.fileLogLevel, "trace");
    EXPECT_TRUE(params.enableRotation);
    EXPECT_EQ(params.maxFileSize, 20 * 1024 * 1024);
    EXPECT_EQ(params.maxFiles, 5);
}

TEST(LoggingParamsTest, DailyRotation) {
    auto params = LoggingParams::defaults()
                      .withFileOutput(true)
                      .withDailyRotation(true);

    EXPECT_TRUE(params.logToFile);
    EXPECT_TRUE(params.rotateDaily);
}
