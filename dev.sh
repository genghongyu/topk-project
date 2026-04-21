#!/bin/bash
set -euo pipefail

# =========================
# Config
# =========================
MAIN_APP="topk_app"

BUILD_DIR_BASE="build"
BUILD_DIR=""
BUILD_TYPE="Debug"

# =========================
# Usage
# =========================
usage() {
    cat <<EOF
===================================
           Dev Tool
===================================
Usage:

  dev.sh build [debug|release]
      Configure & build project

  dev.sh test [unit|integration|smoke|all]
      Run tests by category (default: all)

  dev.sh report
      Run all tests + generate coverage report

  dev.sh clean
      Remove build directory

EOF
    exit 1
}

# =========================
# Helpers
# =========================
remove_build() {
    if [ -d "$BUILD_DIR_BASE" ]; then
        echo "[INFO] remove $BUILD_DIR_BASE directory..."
        rm -rf $BUILD_DIR_BASE
    else
        echo "[INFO] $BUILD_DIR_BASE directory not found, skip"
    fi
}

set_build_dir() {
    local type="${1:-debug}"

    case "$type" in
        debug)
            BUILD_TYPE="Debug"
            BUILD_DIR="${BUILD_DIR_BASE}/debug"
            ;;
        release)
            BUILD_TYPE="Release"
            BUILD_DIR="${BUILD_DIR_BASE}/release"
            ;;
        coverage)
            BUILD_TYPE="Debug"
            BUILD_DIR="${BUILD_DIR_BASE}/coverage"
            ;;
        *)
            echo "[ERROR] invalid build type: $type"
            exit 1
            ;;
    esac
}

# =========================
# Build
# =========================
build() {
    local type="${1:-debug}"

    set_build_dir "$type"

    echo "[INFO] build type: $BUILD_TYPE"
    echo "[INFO] build dir: $BUILD_DIR"

    # =========================
    # coverage switch
    # =========================
    local coverage_flag="OFF"

    if [ "$type" == "coverage" ]; then
        coverage_flag="ON"
    fi

    cmake -S . -B "$BUILD_DIR" \
        -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
        -DENABLE_COVERAGE="$coverage_flag"

    cmake --build "$BUILD_DIR" -j"$(nproc)"
}

# =========================
# Test (CI + local)
# =========================
run_tests() {
    local target="${1:-all}"

    if [ -z "$BUILD_DIR" ]; then
        set_build_dir debug
    fi

    echo "[INFO] running tests: $target"
    echo "[INFO] build dir: $BUILD_DIR"

    case "$target" in
        unit)
            ctest --test-dir "$BUILD_DIR" -L unit --output-on-failure
            ;;
        integration)
            ctest --test-dir "$BUILD_DIR" -L integration --output-on-failure
            ;;
        smoke)
            ctest --test-dir "$BUILD_DIR" -L smoke --output-on-failure
            ;;
        all)
            ctest --test-dir "$BUILD_DIR" --output-on-failure
            ;;
        *)
            echo "[ERROR] unknown test type: $target"
            exit 1
            ;;
    esac
}

# =========================
# Coverage
# =========================
generate_coverage() {
    echo "[INFO] generating coverage report..."

    mkdir -p "$BUILD_DIR/coverage"

    gcovr -r . \
        --exclude-throw-branches \
        --exclude 'tests/.*' \
        --exclude 'build/.*' \
        --exclude 'src/main.cpp' \
        --print-summary \
        --fail-under-line 80 \
        --html --html-details \
        -o "$BUILD_DIR/coverage/index.html"

    echo "[INFO] coverage report: $BUILD_DIR/coverage/index.html"
}

# =========================
# Main logic
# =========================
if [ $# -lt 1 ]; then
    echo "[ERROR] No command provided"
    usage
fi

CMD=$1
shift || true

case "$CMD" in

    build)
        build "${1:-debug}"
        ;;

    test)
        build debug
        run_tests "${1:-all}"
        ;;

    report)
        remove_build
        build coverage
        run_tests all
        generate_coverage
        ;;

    clean)
        remove_build
        ;;

    -h|--help)
        usage
        ;;

    *)
        echo "[ERROR] unknown command: $CMD"
        usage
        ;;
esac