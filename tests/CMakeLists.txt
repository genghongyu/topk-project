# =========================
# GoogleTest dependency
# =========================
find_package(GTest REQUIRED)

# =========================
# Test executable
# =========================
set(TEST_APP test_topk)
add_executable(${TEST_APP}
    test_app_main.cpp
    test_topk_processor.cpp
    test_parse_record.cpp
    test_parse_args.cpp
    test_run_app.cpp
)

target_link_libraries(${TEST_APP}
    PRIVATE ${LIB_NAME}
    GTest::gtest
    GTest::gtest_main
)

# Automatically register all TEST() cases
include(GoogleTest)
gtest_discover_tests(${TEST_APP})