#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#include <string>

// Application configuration model
// Holds runtime parameters parsed from CLI arguments
enum class InputMode {
    File,
    Stdin
};

// Global configuration for application execution
// - top_k      : number of top records to keep
// - file_path  : input file path (used in File mode)
// - mode       : input source selection (File or Stdin)
struct AppConfig {
    int top_k = -1;
    std::string file_path;
    InputMode mode = InputMode::Stdin;

    // validate whether top_k is usable (> 0)
    bool is_topk_valid() const {
        return top_k > 0;
    }
};

#endif // APP_CONFIG_H