# Top-K Records Processor

## 1. Overview

This project implements a command-line tool that extracts the Top-K records with the largest numeric values from a dataset.

Each input line follows the following format:

```
<record_id> <numeric_value>
```

The program supports both file input and standard input (stdin), and is designed for large-scale data processing.

---

## 2. Features

- Top-K computation using a bounded min-heap (O(N log K))
- Streaming input processing (no full dataset loading)
- Supports both:
  - Standard input (`stdin`, e.g., Unix pipelines)
  - File input via command-line argument
- Graceful handling of malformed input (invalid lines are skipped with warnings)
- Modular and testable architecture
- Multi-level testing strategy (unit, integration, and smoke tests)
- CI integration (e.g., GitHub Actions) for automated build and test validation

---

## 3. Environment

This project is developed and tested on WSL (Ubuntu environment).

### Tool Versions

| Tool   | Version |
|--------|--------|
| OS     | WSL (Ubuntu 24.04) |
| CMake  | 3.28.3 |
| g++    | 13.3.0 (Ubuntu 13.3.0-6ubuntu2~24.04.1) |
| Google Test | 1.14.0 (libgtest-dev) |
| gcovr  | 7.0 |

---

## 4. Build & Run

### Build

```bash
./dev.sh build
```

### Output

- Debug build:
  ```
  build/debug/
  ```

- Release build:
  ```
  build/release/
  ```

### Executable

```
build/debug/topk_app
```

### Run

- From file:

```bash
./build/debug/topk_app 3 input.txt
```

- From stdin:

```bash
cat input.txt | ./build/debug/topk_app 3
```

---

## 5. Algorithm

A min-heap of size K is used to maintain the Top-K elements.

- Insert elements until heap size reaches K
- For each new element, if its value is larger than the smallest in heap, replace it

### Complexity

- Time Complexity: O(N log K)
- Space Complexity: O(K)

---

## 6. Design

### Streaming Processing
- Processes input line by line
- Supports large files efficiently

### Separation of Concerns
- `parse_args`: CLI parsing
- `parse_record`: input parsing
- `topk_processor`: core algorithm
- `run_app`: orchestration layer

### Input Abstraction
- Uses `std::istream` for both file and stdin

### Error Handling
- Empty and invalid lines are skipped
- Warnings printed to stderr

---

## 7. Trade-offs

- Min-heap vs sorting → better scalability
- Streaming vs batch → memory efficient
- Robustness vs strict validation → tolerate bad input
- Unordered vs sorted output → avoids extra cost

---

## 8. Testing

- Unit tests: core components
- Integration tests: full pipeline
- Smoke tests: end-to-end execution

Run tests by category (default: all)

```bash
./dev.sh test [unit|integration|smoke|all]
```

---

## 9. Development Workflow

The project includes a unified development script:

```bash
./dev.sh build
./dev.sh test
./dev.sh report
./dev.sh clean
./dev.sh -h|--help
```

### Notes

- Uses CTest for test execution
- Coverage generated via gcovr
- Same commands can be used in CI

### Coverage report

```
build/coverage/coverage/index.html
```

---

## 10. Limitations & Future Work

### Limitations
- Output is not sorted
- Input validation is permissive
- Build/test workflow has some coupling and limited extensibility

### Future Work
- Optional sorted output
- Strict validation mode
- Decouple build/test stages and improve CI configurability

---

## 11. Project Structure

```
.
├── include/
├── src/
├── tests/
├── dev.sh
├── CMakeLists.txt
└── README.md
```

---

## 12. AI Usage Disclosure

AI tools were used for code review and documentation assistance.

All design and implementation decisions are understood and validated.