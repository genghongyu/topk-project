#include <fstream>
#include <string>
#include "record.h"
#include "run_app.h"
#include "parse_record.h"
#include "topk_processor.h"

int run_app(const AppConfig& config,
            std::istream& in,
            std::ostream& out,
            std::ostream& err) {

    std::istream* input = nullptr;
    std::ifstream infile;

    // 1. initialize input source
    //    - file mode: read from file path
    //    - stdin mode: use provided input stream
    if (config.mode == InputMode::File) {
        infile.open(config.file_path);
        if (!infile.is_open()) {
            err << "[run_app] Cannot open file\n";
            return 1;
        }
        input = &infile;
    } else {
        input = &in;
    }

    // 2. validate configuration
    //    - top_k must be valid (> 0)
    if (!config.is_topk_valid()) {
        err << "[ERROR] invalid top_k: " << config.top_k << "\n";
        return 1;
    }

    CTopKProcessor top_k_proc(static_cast<size_t>(config.top_k));

    // 3. process input stream line by line
    std::string line;
    int invalid_count = 0;

    while (std::getline(*input, line)) {

        // skip empty lines
        if (line.empty()) continue;

        Record record;

        // parse each line into a record
        // invalid lines are ignored
        if (!parse_line(line, record)) {
            ++invalid_count;
            continue;
        }

        // add valid record into top-k processor
        top_k_proc.add(record);
    }

    // 4. report invalid input statistics (if any)
    if (invalid_count > 0) {
        err << "[WARN] invalid records skipped: " << invalid_count << "\n";
    }

    // 5. output final top-k results
    auto res = top_k_proc.get_result();
    for (const auto& r : res) {
        out << r.id << "\n";
    }

    return 0;
}