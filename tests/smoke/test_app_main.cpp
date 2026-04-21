#include <gtest/gtest.h>
#include <sstream>
#include "app_main.h"

// ------------------------------
// Normal case: valid input flow
// ------------------------------
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

    int ret = app_main(argc, const_cast<char**>(argv), input, output, err);

    EXPECT_EQ(ret, 0);
    EXPECT_TRUE(err.str().empty());
}

// ------------------------------
// Error case: invalid command-line arguments
// ------------------------------
TEST(AppMainTest, InvalidArgs) {
    const char* argv[] = {"app"};
    int argc = 1;

    std::istringstream input;
    std::ostringstream output;
    std::ostringstream err;

    int ret = app_main(argc, const_cast<char**>(argv), input, output, err);

    EXPECT_EQ(ret, 1);
    EXPECT_FALSE(err.str().empty());
}