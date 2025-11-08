// Copyright (c) 2025 Liu Yue. All rights reserved.

#include <qtl/logging/logger_registry.h>
#include <qtl/logging/level.h>

#include <stdexcept>
#include <vector>

namespace qtl::logging {

LoggerRegistry& LoggerRegistry::instance() {
    static LoggerRegistry registry;
    return registry;
}

void LoggerRegistry::initialize(std::shared_ptr<const LoggingParams> params, std::string sessionDir) {
    // Create sink managers internally based on params
    auto consoleSinkManager = std::make_shared<ConsoleSinkManager>(params);
    auto fileSinkManager = std::make_shared<FileSinkManager>(params, sessionDir);

    // Delegate to the advanced initialize
    initialize(std::move(params), std::move(consoleSinkManager), std::move(fileSinkManager));
}

void LoggerRegistry::initialize(std::shared_ptr<const LoggingParams> params,
                                 std::shared_ptr<ConsoleSinkManager> consoleSinkManager,
                                 std::shared_ptr<FileSinkManager> fileSinkManager) {
    auto& reg = instance();
    reg.mParams = std::move(params);
    reg.mConsoleSinkManager = std::move(consoleSinkManager);
    reg.mFileSinkManager = std::move(fileSinkManager);
    reg.mInitialized = true;
}

std::shared_ptr<spdlog::logger> LoggerRegistry::getLogger(std::string_view name) {
    auto& reg = instance();
    if (!reg.mInitialized) {
        throw std::runtime_error("LoggerRegistry not initialized. Call LoggerRegistry::initialize() first.");
    }

    // Check if logger already exists
    auto existing = spdlog::get(std::string(name));
    if (existing) {
        return existing;
    }

    // Create new logger with configured sinks
    auto consoleSink = reg.mConsoleSinkManager ? reg.mConsoleSinkManager->getConsoleSink() : nullptr;
    auto fileSink = reg.mFileSinkManager ? reg.mFileSinkManager->getFileSink() : nullptr;

    std::vector<std::shared_ptr<spdlog::sinks::sink>> sinks;
    if (consoleSink) {
        sinks.push_back(consoleSink);
    }
    if (fileSink) {
        sinks.push_back(fileSink);
    }

    auto logger = std::make_shared<spdlog::logger>(std::string(name), sinks.begin(), sinks.end());
    logger->set_level(detail::toSpdlogLevel(reg.mParams->logLevel));

    if (fileSink) {
        logger->flush_on(detail::toSpdlogLevel(reg.mParams->fileFlushOn));
    }

    spdlog::register_logger(logger);
    return logger;
}

std::shared_ptr<spdlog::logger> LoggerRegistry::getDefaultLogger() {
    return spdlog::default_logger();
}

void LoggerRegistry::setDefaultLogger(std::shared_ptr<spdlog::logger> logger) {
    spdlog::set_default_logger(std::move(logger));
}

bool LoggerRegistry::isInitialized() {
    return instance().mInitialized;
}

void LoggerRegistry::flushAll() {
    auto& reg = instance();
    if (reg.mFileSinkManager) {
        reg.mFileSinkManager->flushAll();
    }
    spdlog::apply_all([](std::shared_ptr<spdlog::logger> logger) { logger->flush(); });
}

std::shared_ptr<const LoggingParams> LoggerRegistry::getParams() {
    return instance().mParams;
}

std::shared_ptr<ConsoleSinkManager> LoggerRegistry::getConsoleSinkManager() {
    return instance().mConsoleSinkManager;
}

std::shared_ptr<FileSinkManager> LoggerRegistry::getFileSinkManager() {
    return instance().mFileSinkManager;
}

}  // namespace qtl::logging
