#ifndef PARSE_ARGS_H
#define PARSE_ARGS_H

#include "app_config.h"

// Parse CLI arguments and fill AppConfig
// return 0 on success, non-zero on error
int parse_args(int argc, char* argv[], AppConfig& config,  std::ostream& err);

#endif // PARSE_ARGS_H