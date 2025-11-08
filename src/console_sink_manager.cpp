#include <qtl/logging/console_sink_manager.h>
#include <qtl/logging/level.h>

#include <spdlog/sinks/stdout_color_sinks.h>

namespace qtl::logging {

ConsoleSinkManager::ConsoleSinkManager(std::shared_ptr<const LoggingParams> params) : mParams{std::move(params)} {}

std::shared_ptr<spdlog::sinks::sink> ConsoleSinkManager::getConsoleSink() {
    if (!mParams->logToConsole) {
        return nullptr;
    }

    if (!mConsoleSink) {
        mConsoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        mConsoleSink->set_level(detail::toSpdlogLevel(mParams->consoleLogLevel));
        mConsoleSink->set_pattern(mParams->consoleLogPattern);
    }

    return mConsoleSink;
}

}  // namespace qtl::logging
