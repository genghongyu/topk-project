#include <string>
#include <stdexcept>
#include "parse_args.h"

AppConfig parse_args(int argc, char* argv[]) {
    AppConfig config;

    if (argc < 2) {
        throw std::runtime_error("Usage: topk <X> [file_path]");
    }

    const char* top_k_str = argv[1];
    const char* file_path_str = (argc >= 3) ? argv[2] : nullptr;

    try {
        config.top_k = std::stoi(top_k_str);
    } catch (const std::invalid_argument&) {
        throw std::runtime_error("[parse_args] Invalid argument: top_k is not a number");
    } catch (const std::out_of_range&) {
        throw std::runtime_error("[parse_args] Invalid argument: top_k out of range");
    }

    if (config.top_k <= 0) {
        throw std::runtime_error("[parse_args] top_k must be > 0");
    }

    if (file_path_str) {
        config.file_path = file_path_str;
        config.mode = InputMode::File;
    } else {
        config.mode = InputMode::Stdin;
    }

    return config;
}