#include <gtest/gtest.h>
#include <set>
#include "topk_processor.h"

TEST(TopKTest, Basic) {
    CTopKProcessor p(3);

    p.add({1, 100});
    p.add({2, 200});
    p.add({3, 50});
    p.add({4, 300});

    auto res = p.get_result();

    EXPECT_EQ(res.size(), 3);

    // 结果无序，用 set 验证
    std::set<int> ids;
    for (const auto& r : res) {
        ids.insert(r.id);
    }

    std::set<int> expected = {1, 2, 4}; // value 最大的三个
    EXPECT_EQ(ids, expected);
}

// -------- top_k = 0 --------
TEST(TopKTest, TopKZero) {
    CTopKProcessor p(0);

    p.add({1, 100});
    p.add({2, 200});

    auto res = p.get_result();
    EXPECT_TRUE(res.empty());
}

// -------- 少于 K --------
TEST(TopKTest, LessThanK) {
    CTopKProcessor p(5);

    p.add({1, 100});
    p.add({2, 200});

    auto res = p.get_result();

    EXPECT_EQ(res.size(), 2);
}

// -------- 相等 value --------
TEST(TopKTest, EqualValues) {
    CTopKProcessor p(2);

    p.add({1, 100});
    p.add({2, 100});
    p.add({3, 100});

    auto res = p.get_result();

    EXPECT_EQ(res.size(), 2);
}

// -------- 全被淘汰 --------
TEST(TopKTest, AllFilteredOut) {
    CTopKProcessor p(2);

    p.add({1, 100});
    p.add({2, 200});
    p.add({3, 50});   // 不进
    p.add({4, 10});   // 不进

    auto res = p.get_result();

    std::set<int> ids;
    for (const auto& r : res) {
        ids.insert(r.id);
    }

    std::set<int> expected = {1, 2};
    EXPECT_EQ(ids, expected);
}