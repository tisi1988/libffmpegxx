#pragma once

#include <string>
#include <unordered_map>
#include <variant>

namespace libffmpegxx {
namespace utils {
using AVOptions =
    std::unordered_map<std::string, std::variant<int, std::string>>;
};
}; // namespace libffmpegxx
