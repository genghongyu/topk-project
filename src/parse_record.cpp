#include <sstream>
#include "parse_record.h"

bool parse_line(const std::string& line, Record& record) {
    std::istringstream iss(line);

    if (!(iss >> record.id >> record.value)) {
        return false;
    }
    return true;
}