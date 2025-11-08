// Copyright (c) 2025 Liu Yue. All rights reserved.

#pragma once

#include <functional>
#include <string>
#include <string_view>

namespace qtl::logging {

struct StrHash {
    using is_transparent = void;  // Enables heterogeneous lookup

    size_t operator()(std::string_view sv) const noexcept { return std::hash<std::string_view>{}(sv); }
    size_t operator()(const std::string& s) const noexcept { return std::hash<std::string>{}(s); }
};

struct StrEqualTo {
    using is_transparent = void;  // Enables heterogeneous lookup

    bool operator()(std::string_view lhs, std::string_view rhs) const noexcept { return lhs == rhs; }
    bool operator()(const std::string& lhs, const std::string& rhs) const noexcept { return lhs == rhs; }
    bool operator()(std::string_view lhs, const std::string& rhs) const noexcept { return lhs == rhs; }
    bool operator()(const std::string& lhs, std::string_view rhs) const noexcept { return lhs == rhs; }
};

}  // namespace qtl::logging
