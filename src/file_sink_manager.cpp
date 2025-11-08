#include <qtl/logging/file_sink_manager.h>
#include <qtl/logging/level.h>

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>

#include <format>

namespace qtl::logging {

FileSinkManager::FileSinkManager(std::shared_ptr<const LoggingParams> params, std::filesystem::path sessionDir)
  : mParams{std::move(params)}, mSessionDir{std::move(sessionDir)} {
    if (!mSessionDir.empty()) {
        std::filesystem::create_directories(mSessionDir);
    }
}

std::shared_ptr<spdlog::sinks::sink> FileSinkManager::getFileSink() {
    if (!mParams->logToFile) {
        return nullptr;
    }

    // Construct file path
    std::filesystem::path filePath;
    if (!mSessionDir.empty()) {
        filePath = mSessionDir / std::format("{}.log", mParams->logFilePrefix);
    } else {
        filePath = std::format("{}.log", mParams->logFilePrefix);
    }

    auto filePathStr = filePath.string();

    // Check if sink already exists
    auto it = mFileSinks.find(filePathStr);
    if (it != mFileSinks.end()) {
        return it->second;
    }

    // Create appropriate sink based on rotation settings
    std::shared_ptr<spdlog::sinks::sink> fileSink;

    if (mParams->rotateDaily) {
        // Daily rotation: new file every day at midnight
        fileSink = std::make_shared<spdlog::sinks::daily_file_sink_mt>(filePathStr, 0, 0);
    } else if (mParams->enableRotation) {
        // Size-based rotation
        fileSink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
            filePathStr, mParams->maxFileSize, mParams->maxFiles);
    } else {
        // Basic file sink (no rotation)
        fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(filePathStr);
    }

    // Configure sink
    fileSink->set_level(detail::toSpdlogLevel(mParams->fileLogLevel));
    fileSink->set_pattern(mParams->fileLogPattern);

    // Cache and return
    mFileSinks[filePathStr] = fileSink;
    return fileSink;
}

void FileSinkManager::flushAll() {
    for (auto& [_, sink] : mFileSinks) {
        sink->flush();
    }
}

}  // namespace qtl::logging
