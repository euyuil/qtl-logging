// Copyright (c) 2025 Liu Yue. All rights reserved.

#pragma once

#include <qtl/logging/console_sink_manager.h>
#include <qtl/logging/file_sink_manager.h>
#include <qtl/logging/logging_params.h>
#include <qtl/logging/spdlog_config.h>

#include <memory>
#include <string>
#include <string_view>

namespace qtl::logging {

/**
 * Global registry for logger configuration and access.
 *
 * This singleton provides:
 * - Global configuration of default sinks
 * - Logger creation and retrieval by name
 * - Default logger for non-Loggable classes
 *
 * Usage:
 *   // Initialize once at application startup (simple)
 *   LoggerRegistry::initialize(
 *       LoggingParams::defaults()
 *           .withConsoleLevel("info")
 *           .withFileOutput(true)
 *   );
 *
 *   // Get logger from anywhere
 *   auto logger = LoggerRegistry::getLogger("MyComponent");
 *   logger->info("Hello!");
 */
class LoggerRegistry {
public:
    /**
     * Initialize the global logger registry (simplified API).
     * Creates sink managers internally based on LoggingParams.
     * This should be called once at application startup.
     *
     * @param params Logging configuration
     * @param sessionDir Optional directory for log files (e.g., "logs/2025-01-08")
     */
    static void initialize(std::shared_ptr<const LoggingParams> params, std::string sessionDir = "");

    /**
     * Initialize the global logger registry (advanced API).
     * Allows providing custom sink managers for advanced use cases.
     * Most users should use the simplified initialize() instead.
     *
     * @param params Logging configuration
     * @param consoleSinkManager Console sink manager (can be null)
     * @param fileSinkManager File sink manager (can be null)
     */
    static void initialize(std::shared_ptr<const LoggingParams> params,
                           std::shared_ptr<ConsoleSinkManager> consoleSinkManager,
                           std::shared_ptr<FileSinkManager> fileSinkManager);

    /**
     * Get or create a logger by name.
     * The logger will use the globally configured sinks.
     *
     * @param name Logger name
     * @return Shared pointer to logger
     */
    static std::shared_ptr<spdlog::logger> getLogger(std::string_view name);

    /**
     * Get the default logger.
     * Useful for logging from non-Loggable classes.
     */
    static std::shared_ptr<spdlog::logger> getDefaultLogger();

    /**
     * Set the default logger.
     */
    static void setDefaultLogger(std::shared_ptr<spdlog::logger> logger);

    /**
     * Check if the registry has been initialized.
     */
    static bool isInitialized();

    /**
     * Flush all registered loggers.
     */
    static void flushAll();

    // Internal use only
    static std::shared_ptr<const LoggingParams> getParams();
    static std::shared_ptr<ConsoleSinkManager> getConsoleSinkManager();
    static std::shared_ptr<FileSinkManager> getFileSinkManager();

private:
    LoggerRegistry() = default;
    static LoggerRegistry& instance();

    std::shared_ptr<const LoggingParams> mParams;
    std::shared_ptr<ConsoleSinkManager> mConsoleSinkManager;
    std::shared_ptr<FileSinkManager> mFileSinkManager;
    bool mInitialized = false;
};

}  // namespace qtl::logging
