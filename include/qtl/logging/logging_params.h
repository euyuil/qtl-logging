// Copyright (c) 2025 Liu Yue. All rights reserved.

#pragma once

#include <string>

namespace qtl::logging {

/**
 * Configuration parameters for the logging system.
 *
 * This is a simple data structure with no external dependencies.
 * Clients are responsible for populating it from their configuration sources (JSON, YAML, etc.)
 */
struct LoggingParams {
    // Console output settings
    bool logToConsole = true;
    std::string consoleLogLevel = "info";
    std::string consoleLogPattern = "[%Y-%m-%d %H:%M:%S.%f] [%t] [%^%L%$] [%n] %v";

    // File output settings
    bool logToFile = false;
    std::string logFilePrefix = "app";
    std::string fileLogLevel = "trace";
    std::string fileLogPattern = "[%Y-%m-%d %H:%M:%S.%f] [%P] [%t] [%L] [%n] %v";
    std::string fileFlushOn = "info";

    // Rotation settings
    bool enableRotation = false;
    size_t maxFileSize = 10 * 1024 * 1024;  // 10MB default
    size_t maxFiles = 10;
    bool rotateDaily = false;

    // Overall logger level
    std::string logLevel = "trace";

    // Builder-style methods for fluent configuration
    LoggingParams& withConsoleOutput(bool enable = true) {
        logToConsole = enable;
        return *this;
    }

    LoggingParams& withFileOutput(bool enable = true) {
        logToFile = enable;
        return *this;
    }

    LoggingParams& withConsoleLevel(std::string level) {
        consoleLogLevel = std::move(level);
        return *this;
    }

    LoggingParams& withFileLevel(std::string level) {
        fileLogLevel = std::move(level);
        return *this;
    }

    LoggingParams& withLogLevel(std::string level) {
        logLevel = std::move(level);
        return *this;
    }

    LoggingParams& withFilePrefix(std::string prefix) {
        logFilePrefix = std::move(prefix);
        return *this;
    }

    LoggingParams& withRotation(bool enable = true, size_t fileSizeMB = 10, size_t fileCount = 10) {
        enableRotation = enable;
        maxFileSize = fileSizeMB * 1024 * 1024;
        maxFiles = fileCount;
        return *this;
    }

    LoggingParams& withDailyRotation(bool enable = true) {
        rotateDaily = enable;
        return *this;
    }

    LoggingParams& withConsolePattern(std::string pattern) {
        consoleLogPattern = std::move(pattern);
        return *this;
    }

    LoggingParams& withFilePattern(std::string pattern) {
        fileLogPattern = std::move(pattern);
        return *this;
    }

    LoggingParams& withFlushOn(std::string level) {
        fileFlushOn = std::move(level);
        return *this;
    }

    // Factory methods for common configurations
    static LoggingParams defaults() {
        return LoggingParams{};
    }

    static LoggingParams consoleOnly() {
        LoggingParams params;
        params.logToConsole = true;
        params.logToFile = false;
        return params;
    }

    static LoggingParams fileOnly(std::string prefix = "app") {
        LoggingParams params;
        params.logToConsole = false;
        params.logToFile = true;
        params.logFilePrefix = std::move(prefix);
        return params;
    }
};

}  // namespace qtl::logging
