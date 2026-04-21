#ifndef APP_MAIN_H
#define APP_MAIN_H

#include <istream>
#include <ostream>

// Entry point of the application logic (testable main wrapper)
// - Parses CLI arguments
// - Executes main processing pipeline
// - Returns 0 on success, non-zero on error
int app_main(int argc, char* argv[], std::istream& in, std::ostream& out, std::ostream& err);

#endif // APP_MAIN_H