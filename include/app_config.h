#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#include <string>

enum class InputMode {
    File,
    Stdin
};

struct AppConfig {
    int top_k = -1;
    std::string file_path;
    InputMode mode = InputMode::Stdin;

    bool is_topk_valid() const {
        return top_k > 0;
    }
};

#endif // APP_CONFIG_H