#include <string>
#include <iostream>
#include "parse_args.h"

int parse_args(int argc, char* argv[], AppConfig& config, std::ostream& err) {

    // 1. check number of command-line arguments
    if (argc < 2) {
        err << "Usage: topk <X> [file_path]\n";
        return 1;
    }

    // 2. extract arguments
    const char* top_k_str = argv[1];
    const char* file_path_str = (argc >= 3) ? argv[2] : nullptr;

    // 3. parse and validate top_k
    //    - must be a valid integer
    //    - must be greater than 0
    try {
        config.top_k = std::stoi(top_k_str);
    } catch (const std::invalid_argument&) {
        err << "invalid_argument\n";
        return 1;
    } catch (const std::out_of_range&) {
        err << "out_of_range\n";
        return 1;
    }

    if (config.top_k <= 0) {
        err << "[parse_args] top_k must be greater than 0\n";
        return 1;
    }

    // 4. set input mode
    //    - file mode if file path is provided
    //    - otherwise use stdin
    if (file_path_str) {
        config.file_path = file_path_str;
        config.mode = InputMode::File;
    } else {
        config.mode = InputMode::Stdin;
    }

    return 0;
}