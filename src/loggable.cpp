#include <qtl/logging/loggable.h>
#include <qtl/logging/level.h>
#include <qtl/logging/logger_registry.h>

#include <vector>

namespace qtl::logging {

Loggable::Loggable(std::shared_ptr<const LoggingParams> params, std::shared_ptr<ConsoleSinkManager> consoleSinkManager,
                   std::shared_ptr<FileSinkManager> fileSinkManager, std::string loggerName)
  : mConsoleSinkManager{std::move(consoleSinkManager)}, mFileSinkManager{std::move(fileSinkManager)} {
    // Get sinks
    auto consoleSink = mConsoleSinkManager ? mConsoleSinkManager->getConsoleSink() : nullptr;
    auto fileSink = mFileSinkManager ? mFileSinkManager->getFileSink() : nullptr;

    // Collect non-null sinks
    std::vector<std::shared_ptr<spdlog::sinks::sink>> sinks;
    if (consoleSink) {
        sinks.push_back(consoleSink);
    }
    if (fileSink) {
        sinks.push_back(fileSink);
    }

    // Create logger
    mLogger = std::make_shared<spdlog::logger>(std::move(loggerName), sinks.begin(), sinks.end());
    mLogger->set_level(detail::toSpdlogLevel(params->logLevel));

    // Set flush level if file sink exists
    if (fileSink) {
        mLogger->flush_on(detail::toSpdlogLevel(params->fileFlushOn));
    }
}

Loggable::Loggable(std::string loggerName) {
    // Use global registry
    mLogger = LoggerRegistry::getLogger(std::move(loggerName));
    mConsoleSinkManager = LoggerRegistry::getConsoleSinkManager();
    mFileSinkManager = LoggerRegistry::getFileSinkManager();
}

}  // namespace qtl::logging
