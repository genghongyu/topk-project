#include "app_main.h"
#include "parse_args.h"
#include "run_app.h"
#include <iostream>

int app_main(int argc, char* argv[], std::istream& in, std::ostream& out, std::ostream& err) {
    AppConfig config;

    // 1. parse command-line arguments
    int ret = parse_args(argc, argv, config, err);
    if (ret != 0) {
        return ret;
    }

    // 2. run main logic
    return run_app(config, in, out, err);
}