// Copyright (c) 2025 Liu Yue. All rights reserved.

#pragma once

/**
 * qtl-logging - Lightweight C++20 logging library
 *
 * Single-header convenience include for all public APIs.
 *
 * Typical usage:
 *     #include <qtl/logging.h>
 *     using namespace qtl::logging;
 *
 *     // Initialize logging (simple)
 *     LoggerRegistry::initialize(
 *         LoggingParams::defaults()
 *             .withConsoleLevel("info")
 *             .withFileOutput(true)
 *     );
 *
 *     // Use logger
 *     auto logger = LoggerRegistry::getLogger("MyApp");
 *     logger->info("Hello!");
 *
 * This header includes:
 * - LoggingParams: Configuration structure with builder pattern
 * - LoggerRegistry: Global logger registry (simplified initialization)
 * - Loggable: Mixin class with QTL_INFO(), QTL_DEBUG(), etc. macros
 *
 * Advanced/internal (exposed but not typically needed):
 * - ConsoleSinkManager: Console output management (created internally)
 * - FileSinkManager: File output with rotation (created internally)
 */

#include <qtl/logging/logging_params.h>
#include <qtl/logging/logger_registry.h>
#include <qtl/logging/loggable.h>
