#include <gtest/gtest.h>
#include <sstream>
#include <set>
#include "run_app.h"

// -------- 基础功能 --------
TEST(RunAppTest, BasicTopKFromStdin) {
    AppConfig config;
    config.mode = InputMode::Stdin;
    config.top_k = 2;

    std::istringstream input(
        "1 10\n"
        "2 30\n"
        "3 20\n"
        "4 50\n"
    );

    std::ostringstream out;
    std::ostringstream err;

    int ret = run_app(config, input, out, err);

    EXPECT_EQ(ret, 0);
    EXPECT_TRUE(err.str().empty());

    std::istringstream out_stream(out.str());
    std::multiset<int> actual;
    int id;
    while (out_stream >> id) {
        actual.insert(id);
    }

    std::multiset<int> expected = {2, 4};
    EXPECT_EQ(actual, expected);
}

// -------- 非法输入 --------
TEST(RunAppTest, HandleInvalidLines) {
    AppConfig config;
    config.mode = InputMode::Stdin;
    config.top_k = 2;

    std::istringstream input(
        "1 10\n"
        "bad line\n"
        "2 30\n"
        "xxx\n"
        "3 20\n"
    );

    std::ostringstream out;
    std::ostringstream err;

    int ret = run_app(config, input, out, err);

    EXPECT_EQ(ret, 0);

    // 检查 warning
    EXPECT_NE(err.str().find("invalid records skipped: 2"), std::string::npos);

    std::istringstream out_stream(out.str());
    std::multiset<int> actual;
    int id;
    while (out_stream >> id) {
        actual.insert(id);
    }

    std::multiset<int> expected = {2, 3};
    EXPECT_EQ(actual, expected);
}

// -------- top_k = 0 --------
TEST(RunAppTest, TopKIsZero) {
    AppConfig config;
    config.mode = InputMode::Stdin;
    config.top_k = 0;

    std::istringstream input("1 10\n2 20\n");

    std::ostringstream out;
    std::ostringstream err;

    int ret = run_app(config, input, out, err);

    EXPECT_EQ(ret, 1);
    EXPECT_NE(err.str().find("invalid top_k"), std::string::npos);
}

// -------- 输入少于K --------
TEST(RunAppTest, LessThanK) {
    AppConfig config;
    config.mode = InputMode::Stdin;
    config.top_k = 5;

    std::istringstream input(
        "1 10\n"
        "2 20\n"
    );

    std::ostringstream out;
    std::ostringstream err;

    int ret = run_app(config, input, out, err);

    EXPECT_EQ(ret, 0);
    EXPECT_TRUE(err.str().empty());

    std::istringstream out_stream(out.str());
    std::multiset<int> actual;
    int id;
    while (out_stream >> id) {
        actual.insert(id);
    }

    std::multiset<int> expected = {1, 2};
    EXPECT_EQ(actual, expected);
}

// -------- 空输入 --------
TEST(RunAppTest, EmptyInput) {
    AppConfig config;
    config.mode = InputMode::Stdin;
    config.top_k = 3;

    std::istringstream input("");

    std::ostringstream out;
    std::ostringstream err;

    int ret = run_app(config, input, out, err);

    EXPECT_EQ(ret, 0);
    EXPECT_TRUE(out.str().empty());
    EXPECT_TRUE(err.str().empty());
}

// -------- 文件打不开 --------
TEST(RunAppTest, FileOpenFail) {
    AppConfig config;
    config.mode = InputMode::File;
    config.file_path = "non_existent_file.txt";
    config.top_k = 2;

    std::istringstream dummy_in;
    std::ostringstream out;
    std::ostringstream err;

    int ret = run_app(config, dummy_in, out, err);

    EXPECT_EQ(ret, 1);
    EXPECT_NE(err.str().find("Cannot open file"), std::string::npos);
}

// -------- 顺序不保证 --------
TEST(RunAppTest, OrderNotGuaranteed) {
    AppConfig config;
    config.mode = InputMode::Stdin;
    config.top_k = 2;

    std::istringstream input(
        "1 100\n"
        "2 90\n"
        "3 80\n"
    );

    std::ostringstream out;
    std::ostringstream err;

    run_app(config, input, out, err);

    std::string result = out.str();

    EXPECT_TRUE(result.find("1") != std::string::npos);
    EXPECT_TRUE(result.find("2") != std::string::npos);
}