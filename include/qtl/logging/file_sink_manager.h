#pragma once

#include <qtl/logging/logging_params.h>
#include <qtl/util/string_hash.h>

#include <spdlog/sinks/sink.h>

#include <filesystem>
#include <memory>
#include <unordered_map>

namespace qtl::logging {

/**
 * Manages file sinks with support for rotation and multiple log files.
 *
 * Features:
 * - Automatic directory creation
 * - Multiple file sinks (cached by file path)
 * - Size-based rotation (optional)
 * - Daily rotation (optional)
 * - Manual flush all sinks
 */
class FileSinkManager {
public:
    /**
     * Construct FileSinkManager with a session directory.
     *
     * @param params Logging configuration
     * @param sessionDir Directory where log files will be created (e.g., "2025-01-08_DAY")
     *                   If empty, uses current directory
     */
    FileSinkManager(std::shared_ptr<const LoggingParams> params, std::filesystem::path sessionDir = "");

public:
    /**
     * Get a file sink for logging.
     * Returns nullptr if file logging is disabled in params.
     * The sink is cached and reused for the same file path.
     */
    std::shared_ptr<spdlog::sinks::sink> getFileSink();

    /**
     * Flush all managed file sinks.
     * Call this at critical points (e.g., before shutdown, after important events).
     */
    void flushAll();

private:
    std::shared_ptr<const LoggingParams> mParams;
    std::filesystem::path mSessionDir;
    std::unordered_map<std::string, std::shared_ptr<spdlog::sinks::sink>, StrHash, StrEqualTo> mFileSinks;
};

}  // namespace qtl::logging
