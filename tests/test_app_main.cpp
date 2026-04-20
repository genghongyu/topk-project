#include <gtest/gtest.h>
#include <sstream>

#include "app_main.h"

// ---------- 正常流程 ----------
TEST(AppMainTest, NormalFlow) {
    const char* argv[] = {"app", "2"};
    int argc = 2;

    std::istringstream input(
        "1 100\n"
        "2 200\n"
        "3 50\n"
    );

    std::ostringstream output;
    std::ostringstream err;

    int ret = app_main(argc, const_cast<char**>(argv),
                       input, output, err);

    EXPECT_EQ(ret, 0);
    EXPECT_TRUE(err.str().empty());

    std::string out = output.str();

    // 至少包含 top2 的 id（不关心顺序）
    EXPECT_TRUE(out.find("1") != std::string::npos);
    EXPECT_TRUE(out.find("2") != std::string::npos);
}

// ---------- 参数错误 ----------
TEST(AppMainTest, InvalidArgs) {
    const char* argv[] = {"app"}; // 缺参数
    int argc = 1;

    std::istringstream input;
    std::ostringstream output;
    std::ostringstream err;

    int ret = app_main(argc, const_cast<char**>(argv),
                       input, output, err);

    EXPECT_EQ(ret, 1);
    EXPECT_FALSE(err.str().empty());
}

// ---------- 输入非法（parse_record 失败但不中断） ----------
TEST(AppMainTest, BadInputLine) {
    const char* argv[] = {"app", "2"};
    int argc = 2;

    std::istringstream input(
        "1 100\n"
        "bad line\n"
        "2 200\n"
    );

    std::ostringstream output;
    std::ostringstream err;

    int ret = app_main(argc, const_cast<char**>(argv),
                       input, output, err);

    EXPECT_EQ(ret, 0);

    std::string out = output.str();

    EXPECT_TRUE(out.find("1") != std::string::npos);
    EXPECT_TRUE(out.find("2") != std::string::npos);
}

// ---------- 边界：空输入 ----------
TEST(AppMainTest, EmptyInput) {
    const char* argv[] = {"app", "2"};
    int argc = 2;

    std::istringstream input("");
    std::ostringstream output;
    std::ostringstream err;

    int ret = app_main(argc, const_cast<char**>(argv),
                       input, output, err);

    EXPECT_EQ(ret, 0);
    EXPECT_TRUE(output.str().empty());
}