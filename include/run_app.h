#ifndef RUN_APP_H
#define RUN_APP_H

#include <istream>
#include <ostream>
#include "app_config.h"

// Main application execution pipeline
// - initialize input source (file or stdin)
// - validate configuration
// - process input stream and compute Top-K results
// return:
//   0  -> success
//   1  -> error (invalid config, file open failure, etc.)
int run_app(const AppConfig& config, std::istream& in, std::ostream& out, std::ostream& err);

#endif // RUN_APP_H