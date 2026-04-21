#ifndef PARSE_ARGS_H
#define PARSE_ARGS_H

#include "app_config.h"

int parse_args(int argc, char* argv[], AppConfig& config,  std::ostream& err);

#endif // PARSE_ARGS_H