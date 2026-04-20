#ifndef RUN_APP_H
#define RUN_APP_H

#include <istream>
#include <ostream>
#include "app_config.h"

int run_app(const AppConfig& config, std::istream& in, std::ostream& out, std::ostream& err);

#endif // RUN_APP_H