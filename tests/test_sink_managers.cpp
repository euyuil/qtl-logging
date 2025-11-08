#include <qtl/logging.h>

#include <gtest/gtest.h>

#include <filesystem>

using namespace qtl::logging;

TEST(ConsoleSinkManagerTest, DisabledConsole) {
    auto params = std::make_shared<LoggingParams>();
    params->logToConsole = false;

    ConsoleSinkManager manager(params);
    EXPECT_EQ(manager.getConsoleSink(), nullptr);
}

TEST(ConsoleSinkManagerTest, EnabledConsole) {
    auto params = std::make_shared<LoggingParams>();
    params->logToConsole = true;
    params->consoleLogLevel = "info";

    ConsoleSinkManager manager(params);
    auto sink = manager.getConsoleSink();
    EXPECT_NE(sink, nullptr);

    // Second call should return same sink
    EXPECT_EQ(manager.getConsoleSink(), sink);
}

TEST(FileSinkManagerTest, DisabledFile) {
    auto params = std::make_shared<LoggingParams>();
    params->logToFile = false;

    FileSinkManager manager(params);
    EXPECT_EQ(manager.getFileSink(), nullptr);
}

TEST(FileSinkManagerTest, EnabledFile) {
    auto params = std::make_shared<LoggingParams>();
    params->logToFile = true;
    params->logFilePrefix = "test";
    params->fileLogLevel = "debug";

    std::filesystem::path testDir = "test_logs";
    std::filesystem::remove_all(testDir);

    FileSinkManager manager(params, testDir);
    auto sink = manager.getFileSink();
    EXPECT_NE(sink, nullptr);

    // Verify directory was created
    EXPECT_TRUE(std::filesystem::exists(testDir));

    // Second call should return same sink
    EXPECT_EQ(manager.getFileSink(), sink);

    // Cleanup
    std::filesystem::remove_all(testDir);
}

TEST(FileSinkManagerTest, FlushAll) {
    auto params = std::make_shared<LoggingParams>();
    params->logToFile = true;
    params->logFilePrefix = "test_flush";

    std::filesystem::path testDir = "test_logs_flush";
    std::filesystem::remove_all(testDir);

    FileSinkManager manager(params, testDir);
    auto sink = manager.getFileSink();
    EXPECT_NE(sink, nullptr);

    // Flush should not crash
    EXPECT_NO_THROW(manager.flushAll());

    // Cleanup
    std::filesystem::remove_all(testDir);
}
