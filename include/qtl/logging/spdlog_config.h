// Copyright (c) 2025 Liu Yue. All rights reserved.

#pragma once

/**
 * Central spdlog configuration header.
 *
 * This header MUST be included instead of <spdlog/spdlog.h> directly
 * to ensure proper macro configuration before spdlog is included.
 *
 * IMPORTANT: This sets SPDLOG_ACTIVE_LEVEL to enable compile-time filtering.
 * The macro MUST be defined before any spdlog headers are included.
 */

#if !defined(NDEBUG) && !defined(SPDLOG_ACTIVE_LEVEL)
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#endif

#include <spdlog/spdlog.h>
