#include "app_main.h"
#include "parse_args.h"
#include "run_app.h"
#include <iostream>

int app_main(int argc, char* argv[],
             std::istream& in,
             std::ostream& out,
             std::ostream& err) {
    try {
        AppConfig config = parse_args(argc, argv);
        return run_app(config, in, out, err);
    } catch (const std::exception& e) {
        err << e.what() << std::endl;
        return 1;
    }
}