#!/bin/bash
set -euo pipefail

# =========================
# Application config
# =========================
MAIN_APP="topk_app"
BUILD_TYPE=""
BUILD_DIR=""

# =========================
# Usage
# =========================
usage() {
    cat <<EOF
===================================
           Dev Build Tool
===================================
Usage:

  dev.sh debug
      Build application in Debug mode

  dev.sh release
      Build application in Release mode

  dev.sh run [debug|release] [args...]
      Build and run application (default: debug)

  dev.sh rebuild [debug|release]
      Clean build directory and rebuild (default: debug)

  dev.sh test
      Build and run unit tests (Debug mode)

  dev.sh report
      Run unit tests and generate HTML coverage report

  dev.sh clean
      Remove all build artifacts
EOF
    exit 1
}

# =========================
# Utilities
# =========================

remove_build() {
    if [ -d "build" ]; then
        echo "[INFO] remove build directory..."
        rm -rf build
    else
        echo "[INFO] build directory not found, skip"
    fi
}

parse_build_type() {
    case "$1" in
        debug)
            BUILD_TYPE="Debug"
            ;;
        release)
            BUILD_TYPE="Release"
            ;;
        *)
            echo "[ERROR] invalid build type: $1"
            exit 1
            ;;
    esac
}

# =========================
# build (supports coverage flag)
# =========================
build() {
    local COVERAGE_FLAG="${1:-OFF}"

    case "$BUILD_TYPE" in
        Debug) BUILD_DIR="build/debug" ;;
        Release) BUILD_DIR="build/release" ;;
    esac

    echo "[INFO] BUILD_TYPE=$BUILD_TYPE"
    echo "[INFO] BUILD_DIR=$BUILD_DIR"
    echo "[INFO] COVERAGE=$COVERAGE_FLAG"

    cmake -S . -B "$BUILD_DIR" \
        -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
        -DENABLE_COVERAGE="$COVERAGE_FLAG"

    cmake --build "$BUILD_DIR" -j"$(nproc)"
}

run_app() {
    local EXEC="$BUILD_DIR/$MAIN_APP"
    echo "[RUN] $EXEC $*"
    "$EXEC" "$@"
}

run_tests() {
    echo "[INFO] Running unit tests..."
    ctest --test-dir "$BUILD_DIR" --output-on-failure -j"$(nproc)"
}

generate_coverage() {
    echo "[INFO] Generating coverage report..."

    mkdir -p "$BUILD_DIR/coverage"

    gcovr -r . \
        --exclude-throw-branches \
        --exclude 'tests/.*' \
        --exclude 'build/.*' \
        --exclude 'src/main.cpp' \
        --html --html-details \
        -o "$BUILD_DIR/coverage/index.html"

    echo "[INFO] Coverage report:"
    echo "      $BUILD_DIR/coverage/index.html"
}

# =========================
# Main logic
# =========================

if [ $# -lt 1 ]; then
    usage
fi

CMD=$1
shift

case "$CMD" in

    debug)
        parse_build_type debug
        build OFF
        ;;

    release)
        parse_build_type release
        build OFF
        ;;

    run)
        case "${1:-}" in
            debug)
                parse_build_type debug
                shift
                ;;
            release)
                parse_build_type release
                shift
                ;;
            *)
                parse_build_type debug
                ;;
        esac

        build OFF
        run_app "$@"
        ;;

    rebuild)
        parse_build_type "${1:-debug}"
        remove_build
        build OFF
        ;;

    test)
        parse_build_type debug
        build OFF
        run_tests
        ;;

    report)
        parse_build_type debug
        remove_build

        # 🔥 coverage build
        build ON

        run_tests
        generate_coverage
        ;;

    clean)
        remove_build
        ;;

    *)
        usage
        ;;
esac