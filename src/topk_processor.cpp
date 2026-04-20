#include "topk_processor.h"

CTopKProcessor::CTopKProcessor(size_t top_k) : m_top_k(top_k) {}

void CTopKProcessor::add(const Record& record) {
    if (m_top_k == 0) {
        return;
    }

    // Fill heap until it reaches size k
    if (m_heap.size() < m_top_k) {
        m_heap.push(record);
        return;
    }

    const auto& min_elem = m_heap.top();

    // Ignore if not in TopK
    if (record.value <= min_elem.value) {
        return;
    }

    // Replace the smallest element
    m_heap.pop();
    m_heap.push(record);
}

std::vector<Record> CTopKProcessor::get_result() const {
    std::vector<Record> res;
    res.reserve(m_heap.size());

    auto heap_copy = m_heap;

    while (!heap_copy.empty())
    {
        res.push_back(heap_copy.top());
        heap_copy.pop();
    }

    return res;
}