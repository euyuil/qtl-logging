#pragma once

#include <qtl/util/string_hash.h>

#include <spdlog/spdlog.h>

#include <format>
#include <stdexcept>
#include <string_view>
#include <unordered_map>

namespace qtl::logging {

namespace detail {

static const std::unordered_map<std::string_view, spdlog::level::level_enum, StrHash, StrEqualTo> LEVEL_MAP = {
    {"trace", spdlog::level::trace},       {"TRACE", spdlog::level::trace},        // TRACE
    {"debug", spdlog::level::debug},       {"DEBUG", spdlog::level::debug},        // DEBUG
    {"info", spdlog::level::info},         {"INFO", spdlog::level::info},          // INFO
    {"warn", spdlog::level::warn},         {"WARN", spdlog::level::warn},          // WARN
    {"err", spdlog::level::err},           {"ERR", spdlog::level::err},            // ERR
    {"error", spdlog::level::err},         {"ERROR", spdlog::level::err},          // ERROR
    {"critical", spdlog::level::critical}, {"CRITICAL", spdlog::level::critical},  // CRITICAL
};

inline spdlog::level::level_enum toSpdlogLevel(std::string_view level) {
    auto it = LEVEL_MAP.find(level);
    if (it != LEVEL_MAP.end()) {
        return it->second;
    }
    throw std::out_of_range(std::format("Invalid log level: {}", level));
}

}  // namespace detail

}  // namespace qtl::logging
