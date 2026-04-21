#include "topk_processor.h"

CTopKProcessor::CTopKProcessor(size_t top_k)
    : m_top_k(top_k) {}

// Maintain Top-K using min-heap
// - heap size <= K
// - root always stores smallest value in Top-K
void CTopKProcessor::add(const Record& record) {

    if (m_top_k == 0) {
        return;
    }

    // fill heap until reaching K elements
    if (m_heap.size() < m_top_k) {
        m_heap.push(record);
        return;
    }

    const auto& min_elem = m_heap.top();

    // ignore if current record is not large enough
    if (record.value <= min_elem.value) {
        return;
    }

    // replace smallest element in heap
    m_heap.pop();
    m_heap.push(record);
}

// return current Top-K results (unordered)
std::vector<Record> CTopKProcessor::get_result() const {

    std::vector<Record> res;
    res.reserve(m_heap.size());

    auto heap_copy = m_heap;

    while (!heap_copy.empty()) {
        res.push_back(heap_copy.top());
        heap_copy.pop();
    }

    return res;
}