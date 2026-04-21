#include <gtest/gtest.h>
#include <sstream>
#include "parse_args.h"

// ------------------------------
// Normal case: stdin input mode
// ------------------------------
TEST(ParseArgsTest, NormalStdin) {
    const char* argv[] = {"app", "3"};
    int argc = 2;

    AppConfig config;
    std::ostringstream err;

    int ret = parse_args(argc, const_cast<char**>(argv), config, err);

    EXPECT_EQ(ret, 0);
    EXPECT_EQ(config.top_k, 3);
    EXPECT_EQ(config.mode, InputMode::Stdin);
    EXPECT_TRUE(err.str().empty());
}

// ------------------------------
// Normal case: file input mode
// ------------------------------
TEST(ParseArgsTest, NormalFileInput) {
    const char* argv[] = {"app", "5", "input.txt"};
    int argc = 3;

    AppConfig config;
    std::ostringstream err;

    int ret = parse_args(argc, const_cast<char**>(argv), config, err);

    EXPECT_EQ(ret, 0);
    EXPECT_EQ(config.top_k, 5);
    EXPECT_EQ(config.mode, InputMode::File);
    EXPECT_EQ(config.file_path, "input.txt");
    EXPECT_TRUE(err.str().empty());
}

// ------------------------------
// Error case: missing arguments
// ------------------------------
TEST(ParseArgsTest, MissingArguments) {
    const char* argv[] = {"app"};
    int argc = 1;

    AppConfig config;
    std::ostringstream err;

    int ret = parse_args(argc, const_cast<char**>(argv), config, err);

    EXPECT_EQ(ret, 1);
    EXPECT_FALSE(err.str().empty());
}

// ------------------------------
// Error case: not integer top_k
// ------------------------------
TEST(ParseArgsTest, InvalidNumber) {
    const char* argv[] = {"app", "abc"};
    int argc = 2;

    AppConfig config;
    std::ostringstream err;

    int ret = parse_args(argc, const_cast<char**>(argv), config, err);

    EXPECT_EQ(ret, 1);
    EXPECT_FALSE(err.str().empty());
}

// ------------------------------
// Error case: integer overflow top_k
// ------------------------------
TEST(ParseArgsTest, OutOfRange) {
    const char* argv[] = {"app", "92233720368547758099999"}; // guaranteed overflow
    int argc = 2;

    AppConfig config;
    std::ostringstream err;

    int ret = parse_args(argc, const_cast<char**>(argv), config, err);

    EXPECT_EQ(ret, 1);
    EXPECT_FALSE(err.str().empty());
}

// ------------------------------
// Error case: negative top_k
// ------------------------------
TEST(ParseArgsTest, NegativeTopK) {
    const char* argv[] = {"app", "-1"};
    int argc = 2;

    AppConfig config;
    std::ostringstream err;

    int ret = parse_args(argc, const_cast<char**>(argv), config, err);

    EXPECT_EQ(ret, 1);
    EXPECT_FALSE(err.str().empty());
}

// ------------------------------
// Error case: zero top_k
// ------------------------------
TEST(ParseArgsTest, ZeroTopK) {
    const char* argv[] = {"app", "0"};
    int argc = 2;

    AppConfig config;
    std::ostringstream err;

    int ret = parse_args(argc, const_cast<char**>(argv), config, err);

    EXPECT_EQ(ret, 1);
    EXPECT_FALSE(err.str().empty());
}