#include <gtest/gtest.h>
#include <set>
#include "topk_processor.h"

// ------------------------------
// Normal case: basic Top-K functionality
// ------------------------------
TEST(TopKTest, Basic) {
    CTopKProcessor p(3);

    p.add({1, 100});
    p.add({2, 200});
    p.add({3, 50});
    p.add({4, 300});

    auto res = p.get_result();

    EXPECT_EQ(res.size(), 3);

    std::set<int> ids;
    for (const auto& r : res) {
        ids.insert(r.id);
    }

    std::set<int> expected = {1, 2, 4};
    EXPECT_EQ(ids, expected);
}

// ------------------------------
// Edge case: Top-K is zero
// ------------------------------
TEST(TopKTest, TopKZero) {
    CTopKProcessor p(0);

    p.add({1, 100});
    p.add({2, 200});

    auto res = p.get_result();
    EXPECT_TRUE(res.empty());
}

// ------------------------------
// Edge case: input size smaller than K
// ------------------------------
TEST(TopKTest, LessThanK) {
    CTopKProcessor p(5);

    p.add({1, 100});
    p.add({2, 200});

    auto res = p.get_result();

    EXPECT_EQ(res.size(), 2);
}

// ------------------------------
// Edge case: equal values handling
// ------------------------------
TEST(TopKTest, EqualValues) {
    CTopKProcessor p(2);

    p.add({1, 100});
    p.add({2, 100});
    p.add({3, 100});

    auto res = p.get_result();

    EXPECT_EQ(res.size(), 2);
}

// ------------------------------
// Normal case: all lower values filtered out
// ------------------------------
TEST(TopKTest, AllFilteredOut) {
    CTopKProcessor p(2);

    p.add({1, 100});
    p.add({2, 200});
    p.add({3, 50});
    p.add({4, 10});

    auto res = p.get_result();

    std::set<int> ids;
    for (const auto& r : res) {
        ids.insert(r.id);
    }

    std::set<int> expected = {1, 2};
    EXPECT_EQ(ids, expected);
}