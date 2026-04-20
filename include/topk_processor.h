#ifndef TOPK_PROCESSOR_H
#define TOPK_PROCESSOR_H

#include <queue>
#include <vector>
#include <cstddef>
#include "record.h"

class CTopKProcessor {
public:
    explicit CTopKProcessor(size_t top_k);

    void add(const Record& record);

    std::vector<Record> get_result() const;

    size_t size() const noexcept {
        return m_heap.size();
    }
private:
    struct MinHeapCompare {
        bool operator()(const Record& a, const Record& b) const noexcept {
            return a.value > b.value; // min heap
        }
    };

    size_t m_top_k{0};
    std::priority_queue<Record, std::vector<Record>, MinHeapCompare> m_heap;
};

#endif // TOPK_PROCESSOR_H