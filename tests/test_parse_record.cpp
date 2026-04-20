#include <gtest/gtest.h>
#include "parse_record.h"

// -------- 正常输入 --------
TEST(ParseRecordTest, ValidLine) {
    std::string line = "1 100";
    Record r;

    bool ok = parse_line(line, r);

    EXPECT_TRUE(ok);
    EXPECT_EQ(r.id, 1);
    EXPECT_EQ(r.value, 100);
}

// -------- 多空格 --------
TEST(ParseRecordTest, ExtraSpaces) {
    std::string line = "   2    200   ";
    Record r;

    bool ok = parse_line(line, r);

    EXPECT_TRUE(ok);
    EXPECT_EQ(r.id, 2);
    EXPECT_EQ(r.value, 200);
}

// -------- 非法字符串 --------
TEST(ParseRecordTest, InvalidLine) {
    std::string line = "abc def";
    Record r;

    bool ok = parse_line(line, r);

    EXPECT_FALSE(ok);
}

// -------- 部分非法 --------
TEST(ParseRecordTest, PartialInvalid) {
    std::string line = "1 abc";
    Record r;

    bool ok = parse_line(line, r);

    EXPECT_FALSE(ok);
}

// -------- 空字符串 --------
TEST(ParseRecordTest, EmptyLine) {
    std::string line = "";
    Record r;

    bool ok = parse_line(line, r);

    EXPECT_FALSE(ok);
}

// -------- 只有一个字段 --------
TEST(ParseRecordTest, MissingField) {
    std::string line = "1";
    Record r;

    bool ok = parse_line(line, r);

    EXPECT_FALSE(ok);
}

// -------- 多余字段（允许）--------
TEST(ParseRecordTest, ExtraFields) {
    std::string line = "3 300 extra";
    Record r;

    bool ok = parse_line(line, r);

    // 当前实现只读前两个字段 → 应该成功
    EXPECT_TRUE(ok);
    EXPECT_EQ(r.id, 3);
    EXPECT_EQ(r.value, 300);
}