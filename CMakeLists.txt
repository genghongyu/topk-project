cmake_minimum_required(VERSION 3.10)

# =========================
# Project definition
# =========================
project(topk LANGUAGES CXX)

# Coverage option
option(ENABLE_COVERAGE "Enable coverage flags" OFF)

# =========================
# C++ standard config
# =========================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# =========================
# Coverage flags
# =========================
if(ENABLE_COVERAGE)
    message(STATUS "Coverage enabled")

    add_compile_options(--coverage -O0 -g)
    add_link_options(--coverage)
endif()

# =========================
# Core library (business logic)
# =========================
set(LIB_NAME topk_lib)
add_library(${LIB_NAME}
    src/app_main.cpp
    src/parse_args.cpp
    src/parse_record.cpp
    src/run_app.cpp
    src/topk_processor.cpp
)

target_include_directories(${LIB_NAME}
    PUBLIC include
)

target_compile_options(${LIB_NAME}
    PRIVATE -Wall -Wextra -Wpedantic)

# =========================
# Main application
# =========================
set(APP_NAME topk_app)
add_executable(${APP_NAME}
    src/main.cpp
)

target_link_libraries(${APP_NAME}
    PRIVATE ${LIB_NAME}
)

# =========================
# Testing
# =========================
enable_testing()
add_subdirectory(tests)