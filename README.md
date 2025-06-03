# C++/Python Tool Template

A reusable template for building a C++ commandline tool with a Python CLI wrapper, complete with GitHub Actions for CI, 
Docker release, and Conda release. This template lets you jump straight into implementing a C++ tool without setting 
up all the build, test, container, and packaging boilerplate code from scratch.

Template Repo Combines:
- A **C++ executable** (`linecount` - compiled with CMake)
- A **Python wrapper** (`aligncount` - installed via pip)
- Built-in **unit tests** (C++ and Python)
- A **multi-stage Dockerfile** (builds, tests, and packages a wheel)
- A **Conda recipe** (creates a cross-platform packages for Linux/macOS)
- GitHub Actions for CI, Docker release, and Conda release


## [TODO] How To

---

## Repository Layout

    ├── CMakeLists.txt
    ├──.github
    │   └── workflows
    │       ├── ci.yml
    │       ├── conda_release.yml
    │       └── docker_release.yml
    ├── setup.py
    ├── setup.cfg
    ├── pyproject.toml
    ├── src/
    │   └── main.cpp              ← C++ “linecount” implementation
    ├── cli/
    │   ├── __init__.py
    │   └── entrypoint.py         ← Python wrapper (“aligncount” console script)
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

To build manually while at repo root:

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

To install in a virtualenv while at repo root:

    python3 -m venv venv
    source venv/bin/activate
    pip install --upgrade pip setuptools scikit-build pytest
    pip install .
    # Now you have:
    #   - linecount  (C++ binary, in venv/bin)
    #   - aligncount (Python wrapper, in venv/bin)

Run the wrapper (just counts file lines that don't start with `@`):

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
   - Installs runtime dependencies (Python, pip)
   - Installs the wheel (placing linecount and aligncount into /usr/local/bin)
   - Runs the Python unit test (pytest test_wrapper.py)
   - Sets ENTRYPOINT ["aligncount"]

Build and test in one command:

    docker build -t your-org/aligncount-demo:latest .

Run:

    docker run --rm your-org/aligncount-demo:latest --help

---

## Conda-Build Recipe

The `conda-recipe/meta.yaml` contains instructions for the conda-build command to create the conda package for different 
platforms, skipping windows. The recipe expects certain environment variables to be set, which are set by the GitHub 
action `conda_release.yml`. The expected environment variables are:

```
REPO_NAME   # Name of the repo, e.g. cpp-python-tool-template
VERSION     # Tool version, e.g. 0.0.1
TAR_URL     # URL to the repo archive, e.g. https://github.com/user/foo/archive/refs/tags/v0.0.1.tar.gz
SHA256      # SHA256 checksum of the repo archive
REPO_HOME   # URL to the repo home, e.g. https://github.com/user/foo
```

---

## GitHub Actions

### .github/workflows/ci.yml

This workflow is triggered whenever something is commited to the main branch. It builds the tool and runs the unit tests.

### .github/workflows/docker_release.yml

This workflow is triggered whenever a new tag for the repo is released. It builds the docker image for linux/amd64 
and publishes it to the docker hub registry.

The workflow assumes your docker hub username and token are stored as repository secrets under `DOCKERHUB_USERNAME` 
and `DOCKERHUB_TOKEN`.  

### .github/workflows/conda_release.yml

This workflow is triggered whenever a new tag for the repo is released. It uses the conda-build recipe to create and 
publish conda packages to your personal conda channel. The workflow builds conda packages for linux and mac platforms 
running on x86 and arm architecture. The workflow publishes the conda package under the repo name, 
e.g. cpp-python-tool-template. 

The workflow assumes your anaconda username and token are stored as repository secrets under `ANACONDA_USER` 
and `ANACONDA_TOKEN`.

## .gitignore

Included patterns to ignore:
- macOS system files (.DS_Store, ._*)
- CMake and CLion build directories (build/, cmake-build-debug/, _skbuild/, wheelhouse/)
- Python venvs and caches (venv/, __pycache__/, .pytest_cache/, *.egg-info/)
- IDE files (PyCharm/CLion .idea/, *.iml)
- Conda build artifacts (conda-bld/, pkgs/)

---

## What’s Next

- Customize: Adapt this template to use the https://github.com/filipdutescu/modern-cpp-template, and include htslib
- Write a how-to section detailing how to clone the repo and make any necessary edits

With this template, you have a fully tested building, packaging, and distribution pipeline out of the box. Happy coding!
