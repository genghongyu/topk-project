#include <gtest/gtest.h>
#include "parse_record.h"

// ------------------------------
// Normal case: normal input line
// ------------------------------
TEST(ParseRecordTest, ValidLine) {
    std::string line = "1 100";
    Record r;

    auto ret = parse_line(line, r);

    EXPECT_TRUE(ret);
    EXPECT_EQ(r.id, 1);
    EXPECT_EQ(r.value, 100);
}

// ------------------------------
// Normal case: extra spaces in input
// ------------------------------
TEST(ParseRecordTest, ExtraSpaces) {
    std::string line = "   2    200   ";
    Record r;

    auto ret = parse_line(line, r);

    EXPECT_TRUE(ret);
    EXPECT_EQ(r.id, 2);
    EXPECT_EQ(r.value, 200);
}

// ------------------------------
// Error case: non-numeric input
// ------------------------------
TEST(ParseRecordTest, InvalidLine) {
    std::string line = "abc def";
    Record r;

    auto ret = parse_line(line, r);

    EXPECT_FALSE(ret);
}

// ------------------------------
// Error case: partially invalid input
// ------------------------------
TEST(ParseRecordTest, PartialInvalid) {
    std::string line = "1 abc";
    Record r;

    auto ret = parse_line(line, r);

    EXPECT_FALSE(ret);
}

// ------------------------------
// Edge case: empty input line
// ------------------------------
TEST(ParseRecordTest, EmptyLine) {
    std::string line = "";
    Record r;

    auto ret = parse_line(line, r);

    EXPECT_FALSE(ret);
}

// ------------------------------
// Error case: missing second field
// ------------------------------
TEST(ParseRecordTest, MissingField) {
    std::string line = "1";
    Record r;

    auto ret = parse_line(line, r);

    EXPECT_FALSE(ret);
}

// ------------------------------
// Normal case: extra fields are ignored
// ------------------------------
TEST(ParseRecordTest, ExtraFields) {
    std::string line = "3 300 extra";
    Record r;

    auto ret = parse_line(line, r);

    EXPECT_TRUE(ret);
    EXPECT_EQ(r.id, 3);
    EXPECT_EQ(r.value, 300);
}