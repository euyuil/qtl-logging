#pragma once

#if !defined(NDEBUG)
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#endif

#include <qtl/logging/console_sink_manager.h>
#include <qtl/logging/file_sink_manager.h>
#include <qtl/logging/logging_params.h>

#include <spdlog/spdlog.h>

#include <memory>
#include <string>

namespace qtl::logging {

/**
 * Mixin class that provides logging capability to derived classes.
 *
 * Usage:
 *   class MyClass : public Loggable {
 *   public:
 *       MyClass() : Loggable(...) {
 *           QTL_INFO("MyClass created");
 *       }
 *   };
 *
 * The Loggable class creates a logger with specified sinks and manages its lifecycle.
 */
class Loggable {
public:
    /**
     * Construct Loggable with explicit dependencies.
     *
     * @param params Logging configuration
     * @param consoleSinkManager Console sink manager (can be null)
     * @param fileSinkManager File sink manager (can be null)
     * @param loggerName Name for this logger instance
     */
    Loggable(std::shared_ptr<const LoggingParams> params, std::shared_ptr<ConsoleSinkManager> consoleSinkManager,
             std::shared_ptr<FileSinkManager> fileSinkManager, std::string loggerName = "Loggable");

    /**
     * Construct Loggable with just a logger name.
     * Uses the globally configured sinks from LoggerRegistry.
     *
     * @param loggerName Name for this logger instance
     */
    explicit Loggable(std::string loggerName = "Loggable");

protected:
    /**
     * Get the logger instance.
     * Use the QTL_* macros instead of calling this directly.
     */
    spdlog::logger& logger() const { return *mLogger; }

private:
    std::shared_ptr<ConsoleSinkManager> mConsoleSinkManager;
    std::shared_ptr<FileSinkManager> mFileSinkManager;
    std::shared_ptr<spdlog::logger> mLogger;
};

// Convenience macros for logging
#define QTL_TRACE(...) SPDLOG_LOGGER_TRACE(&this->logger(), __VA_ARGS__)
#define QTL_DEBUG(...) SPDLOG_LOGGER_DEBUG(&this->logger(), __VA_ARGS__)
#define QTL_INFO(...) SPDLOG_LOGGER_INFO(&this->logger(), __VA_ARGS__)
#define QTL_WARN(...) SPDLOG_LOGGER_WARN(&this->logger(), __VA_ARGS__)
#define QTL_ERROR(...) SPDLOG_LOGGER_ERROR(&this->logger(), __VA_ARGS__)
#define QTL_CRITICAL(...) SPDLOG_LOGGER_CRITICAL(&this->logger(), __VA_ARGS__)

}  // namespace qtl::logging
