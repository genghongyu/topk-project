#include <fstream>
#include <string>
#include "record.h"
#include "run_app.h"
#include "parse_record.h"
#include "topk_processor.h"

int run_app(const AppConfig& config, std::istream& in, std::ostream& out, std::ostream& err) {
    std::istream* input = nullptr;
    std::ifstream infile;

    // check input source
    if (config.mode == InputMode::File) {
        infile.open(config.file_path);
        if (!infile.is_open()) {
            err << "[run_app] Cannot open file\n";
            return 1;
        }

        input = &infile;
    } else {
        // stdin mode
        input = &in;
    }

    // top_k processing
    if (!config.is_topk_valid()) {
        err << "[ERROR] invalid top_k: " << config.top_k << "\n";
        return 1;
    }
    CTopKProcessor top_k_proc(static_cast<size_t>(config.top_k));

    // stream processing loop
    std::string line;
    int invalid_count = 0;
    while (std::getline(*input, line))
    {
        // skip empty lines
        if (line.empty()) continue;

        Record record;
        // ignore invalid lines
        if (!parse_line(line, record)) {
            ++invalid_count;
            continue;
        }

        // feed record into top-k heap structure
        top_k_proc.add(record);
    }

    // print invalid record count
    if (invalid_count > 0) {
        err << "[WARN] invalid records skipped: " << invalid_count << "\n";
    }

    // print result
    auto res = top_k_proc.get_result();
    for(const auto& r : res) {
        out << r.id << "\n";
    } 

    return 0;
}