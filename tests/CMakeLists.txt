# =========================
# GoogleTest dependency
# =========================
find_package(GTest REQUIRED)

include(GoogleTest)

# =========================
# Unit Tests
# =========================
set(TEST_UNIT_EXE test_unit)
add_executable(${TEST_UNIT_EXE}
    unit/test_topk_processor.cpp
    unit/test_parse_record.cpp
    unit/test_parse_args.cpp
)

target_link_libraries(${TEST_UNIT_EXE}
    PRIVATE
        ${LIB_NAME}
        GTest::gtest
        GTest::gtest_main
)

gtest_discover_tests(${TEST_UNIT_EXE} PROPERTIES LABELS "unit")

# =========================
# Integration Tests
# =========================
set(TEST_IT_EXE test_integration)
add_executable(${TEST_IT_EXE}
    integration/test_run_app.cpp
)

target_link_libraries(${TEST_IT_EXE}
    PRIVATE
        ${LIB_NAME}
        GTest::gtest
        GTest::gtest_main
)

gtest_discover_tests(${TEST_IT_EXE} PROPERTIES LABELS "integration")

# =========================
# Smoke Tests
# =========================
set(TEST_SMOKE_EXE test_smoke)
add_executable(${TEST_SMOKE_EXE}
    smoke/test_app_main.cpp
)

target_link_libraries(${TEST_SMOKE_EXE}
    PRIVATE
        ${LIB_NAME}
        GTest::gtest
        GTest::gtest_main
)

gtest_discover_tests(${TEST_SMOKE_EXE} PROPERTIES LABELS "smoke")