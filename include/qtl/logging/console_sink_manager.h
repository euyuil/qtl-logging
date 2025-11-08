#pragma once

#include <qtl/logging/logging_params.h>

#include <spdlog/sinks/sink.h>

#include <memory>

namespace qtl::logging {

/**
 * Manages a shared console sink for all loggers.
 *
 * This class provides a thread-safe colored console sink that can be shared
 * across multiple loggers. The sink is lazily initialized on first access.
 */
class ConsoleSinkManager {
public:
    explicit ConsoleSinkManager(std::shared_ptr<const LoggingParams> params);

public:
    /**
     * Get the shared console sink.
     * Returns nullptr if console logging is disabled in params.
     */
    std::shared_ptr<spdlog::sinks::sink> getConsoleSink();

private:
    std::shared_ptr<const LoggingParams> mParams;
    std::shared_ptr<spdlog::sinks::sink> mConsoleSink;
};

}  // namespace qtl::logging
