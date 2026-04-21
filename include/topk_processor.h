#ifndef TOPK_PROCESSOR_H
#define TOPK_PROCESSOR_H

#include <queue>
#include <vector>
#include <cstddef>
#include "record.h"

// Top-K processor using min-heap
// Keeps only the K largest records by value
class CTopKProcessor {
public:
    explicit CTopKProcessor(size_t top_k);

    // Add one record into the processor
    // Maintains Top-K invariant internally
    void add(const Record& record);

    // Get current Top-K result (unordered)
    std::vector<Record> get_result() const;

    // current heap size
    size_t size() const noexcept {
        return m_heap.size();
    }

private:
    // min-heap comparator (smallest value at top)
    struct MinHeapCompare {
        bool operator()(const Record& a, const Record& b) const noexcept {
            return a.value > b.value;
        }
    };

    size_t m_top_k{0};
    std::priority_queue<Record, std::vector<Record>, MinHeapCompare> m_heap;
};

#endif // TOPK_PROCESSOR_H