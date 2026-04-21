#ifndef PARSE_RECORD_H
#define PARSE_RECORD_H

#include <string>
#include "record.h"

// Parse one input line into Record
// return true if format is valid, otherwise false
bool parse_line(const std::string& line, Record& record);

#endif // PARSE_RECORD_H