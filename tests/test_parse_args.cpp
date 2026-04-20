#include <gtest/gtest.h>
#include "parse_args.h"

// -------- 正常：stdin 模式 --------
TEST(ParseArgsTest, ValidStdin) {
    const char* argv[] = {"app", "3"};
    int argc = 2;

    AppConfig config = parse_args(argc, const_cast<char**>(argv));

    EXPECT_EQ(config.top_k, 3);
    EXPECT_EQ(config.mode, InputMode::Stdin);
}

// -------- 正常：文件模式 --------
TEST(ParseArgsTest, ValidFileInput) {
    const char* argv[] = {"app", "5", "input.txt"};
    int argc = 3;

    AppConfig config = parse_args(argc, const_cast<char**>(argv));

    EXPECT_EQ(config.top_k, 5);
    EXPECT_EQ(config.mode, InputMode::File);
    EXPECT_EQ(config.file_path, "input.txt");
}

// -------- 参数不足 --------
TEST(ParseArgsTest, MissingArguments) {
    const char* argv[] = {"app"};
    int argc = 1;

    EXPECT_THROW(parse_args(argc, const_cast<char**>(argv)), std::runtime_error);
}

// -------- 非数字 --------
TEST(ParseArgsTest, InvalidNumber) {
    const char* argv[] = {"app", "abc"};
    int argc = 2;

    EXPECT_THROW(parse_args(argc, const_cast<char**>(argv)), std::runtime_error);
}

// -------- 越界 --------
TEST(ParseArgsTest, OutOfRange) {
    const char* argv[] = {"app", "999999999999999999999"};
    int argc = 2;

    EXPECT_THROW(parse_args(argc, const_cast<char**>(argv)), std::runtime_error);
}

// -------- 负数 --------
TEST(ParseArgsTest, NegativeTopK) {
    const char* argv[] = {"app", "-1"};
    int argc = 2;

    EXPECT_THROW(parse_args(argc, const_cast<char**>(argv)), std::runtime_error);
}

// -------- 0 --------
TEST(ParseArgsTest, ZeroTopK) {
    const char* argv[] = {"app", "0"};
    int argc = 2;

    EXPECT_THROW(parse_args(argc, const_cast<char**>(argv)), std::runtime_error);
}