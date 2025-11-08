#pragma once

/**
 * qtl-logging - Lightweight C++20 logging library
 *
 * Single-header convenience include for all public APIs.
 *
 * Usage:
 *     #include <qtl/logging.h>
 *     using namespace qtl::logging;
 *
 * This header includes:
 * - LoggingParams: Configuration structure with builder pattern
 * - ConsoleSinkManager: Console output management
 * - FileSinkManager: File output with rotation support
 * - LoggerRegistry: Global logger registry (singleton pattern)
 * - Loggable: Mixin class with QTL_INFO(), QTL_DEBUG(), etc. macros
 */

#include <qtl/logging/logging_params.h>
#include <qtl/logging/console_sink_manager.h>
#include <qtl/logging/file_sink_manager.h>
#include <qtl/logging/logger_registry.h>
#include <qtl/logging/loggable.h>
