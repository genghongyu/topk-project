#include <gtest/gtest.h>
#include <sstream>
#include <fstream>
#include <set>
#include "run_app.h"

// ------------------------------
// Normal case: basic top-k from stdin
// ------------------------------
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

// ------------------------------
// Normal case: read input from file successfully
// ------------------------------
TEST(RunAppTest, BasicTopKFromFile) {
    AppConfig config;
    config.mode = InputMode::File;
    config.file_path = "test_input.txt";
    config.top_k = 2;

    // prepare a real temp file
    std::ofstream file(config.file_path);
    file << "1 10\n";
    file << "2 30\n";
    file << "3 20\n";
    file << "4 50\n";
    file.close();

    std::istringstream dummy_in;
    std::ostringstream out;
    std::ostringstream err;

    int ret = run_app(config, dummy_in, out, err);

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

    // cleanup
    std::remove(config.file_path.c_str());
}

// ------------------------------
// Error case: invalid input lines
// ------------------------------
TEST(RunAppTest, HandleInvalidLines) {
    AppConfig config;
    config.mode = InputMode::Stdin;
    config.top_k = 2;

    std::istringstream input(
        "1 10\n"
        "bad line\n"
        "2 30\n"
        "\n" // empty line
        "xxx\n"
        "3 20\n"
    );

    std::ostringstream out;
    std::ostringstream err;

    int ret = run_app(config, input, out, err);

    EXPECT_EQ(ret, 0);

    // check warning message
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

// ------------------------------
// Error case: invalid configuration (top_k = 0)
// ------------------------------
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

// ------------------------------
// Normal case: input size smaller than k
// ------------------------------
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

// ------------------------------
// Edge case: empty input
// ------------------------------
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

// ------------------------------
// Error case: file cannot be opened
// ------------------------------
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

// ------------------------------
// Edge case: output order is not guaranteed
// ------------------------------
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