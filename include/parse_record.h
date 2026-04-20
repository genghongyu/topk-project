#ifndef PARSE_RECORD_H
#define PARSE_RECORD_H

#include <string>
#include "record.h"

bool parse_line(const std::string& line, Record& record);

#endif // PARSE_RECORD_H