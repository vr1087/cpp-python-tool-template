# C++/Python Tool Template

A reusable starter template for building a CLI tool that combines:

- A **C++ executable** (compiled with CMake)
- A **Python wrapper** (installed via pip)
- Built-in **unit tests** (C++ and Python)
- A **multi-stage Dockerfile** (builds, tests, and packages a wheel)
- [TODO] A **Conda recipe** (creates a cross-platform package for Linux/macOS)

This template lets you jump straight into developing bioinformatics or similar tools without wiring up all the build, test, container, and packaging boilerplate from scratch.

---

## Repository Layout

    ├── CMakeLists.txt
    ├── setup.py
    ├── setup.cfg
    ├── pyproject.toml
    ├── src/
    │   └── main.cpp             ← C++ “linecount” implementation
    ├── cli/
    │   ├── __init__.py
    │   └── entrypoint.py        ← Python wrapper (“aligncount” console script)
    ├── tests/
    │   ├── cpp/
    │   │   └── test_main.cpp     ← C++ unit test (Doctest + CTest)
    │   └── python/
    │       └── test_wrapper.py   ← Python unit test for format_output()
    ├── Dockerfile                ← Multi-stage Docker build + test
    └── .gitignore

---

## C++ Executable (linecount)

- Source: src/main.cpp
- Build system: CMake (CMakeLists.txt)
- Functionality: Counts non-header lines (simulates an “alignment counter”)
- Unit test: tests/cpp/test_main.cpp using Doctest; run via CTest

To build manually:

    mkdir build
    cd build
    cmake .. -DCMAKE_BUILD_TYPE=Release
    cmake --build . --parallel $(nproc)
    # Now ./linecount is available in build/
    ./linecount sample.sam

---

## Python Wrapper (aligncount)

- Source: cli/entrypoint.py
- Entry point: aligncount console script (configured in setup.py)
- Logic:
  1. Parses subcommands (count-mapped or count-unmapped)
  2. Checks that the input SAM file exists and is non-empty
  3. Calls `linecount <path>` and formats output via format_output(cmd, raw_bytes)

To install in a virtualenv:

    python3 -m venv venv
    source venv/bin/activate
    pip install --upgrade pip setuptools scikit-build pytest
    pip install .
    # Now you have:
    #   - linecount  (C++ binary, in venv/bin)
    #   - aligncount (Python wrapper, in venv/bin)

Run the wrapper (just counts file lines):

    aligncount count-mapped -i sample.sam
    aligncount count-unmapped -i sample.sam

---

## Unit Tests

### C++ Tests

- Framework: Doctest (fetched via ExternalProject_Add in CMake)
- Test file: tests/cpp/test_main.cpp
- Run:

    cd build
    ctest --output-on-failure

### Python Tests

- Framework: pytest
- Test file: tests/python/test_wrapper.py
- Run:

    source venv/bin/activate
    pytest -q

---

## Dockerfile

A multi-stage Dockerfile builds and tests everything, then produces a minimal runtime image:

1. Builder stage (ubuntu:22.04):
   - Installs build-time dependencies (CMake, Git, Python dev headers, pip)
   - Compiles C++ (linecount) and runs CTest
   - Builds a Python wheel (bundles linecount + aligncount)

2. Final stage (ubuntu:22.04):
   - Installs runtime dependencies (Python, pip, samtools)
   - Installs the wheel (placing linecount and aligncount into /usr/local/bin)
   - Runs the Python unit test (pytest test_wrapper.py)
   - Sets ENTRYPOINT ["aligncount"]

Build and test in one command:

    docker build -t your-org/aligncount-demo:latest .

Run:

    docker run --rm your-org/aligncount-demo:latest --help

---

## [TODO] Conda Recipe

---

## .gitignore

Included patterns to ignore:
- macOS system files (.DS_Store, ._*)
- CMake and CLion build directories (build/, cmake-build-debug/, _skbuild/, wheelhouse/)
- Python venvs and caches (venv/, __pycache__/, .pytest_cache/, *.egg-info/)
- IDE files (PyCharm/CLion .idea/, *.iml)
- Conda build artifacts (conda-bld/, pkgs/)

---

## What’s Next

- GitHub Actions: Add workflows to automatically build/test on every push/PR to main, and to release Docker images and Conda packages when tags are pushed.
- Customize: Adapt this template to any C++/Python CLI project by renaming targets, adjusting dependencies, and updating metadata (license, homepage, etc).

With this template, you have a fully tested building, packaging, and distribution pipeline out of the box. Happy coding!
