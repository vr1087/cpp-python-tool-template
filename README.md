# C++/Python Tool Template

A reusable template for building a C++ commandline tool (using HTSlib) with a Python CLI wrapper, complete with GitHub 
Actions for CI, Docker release, and Conda release. This template lets you jump straight into implementing a C++ tool 
without setting up all the build, test, container, and packaging boilerplate code from scratch.

Template Repo Combines:
- A **C++ executable** (`aligncount_cpp` - compiled with CMake)
- A **Python wrapper** (`aligncount` - installed via pip)
- Built-in **unit tests** (C++ and Python)
- A **multi-stage Dockerfile** (builds, tests, and packages a wheel)
- A **Conda recipe** (creates a cross-platform packages for Linux/macOS)
- GitHub Actions for CI, Docker/Conda release, styling, and documentation


## [TODO] How to use this template

---
## Tool Descriptions
### Python Wrapper (aligncount)
- Source: cli/entrypoint.py
- Entry point: aligncount console script (configured in setup.py)
- Functionality:
  - Parses subcommands (count-mapped or count-unmapped)
  - Checks that the input SAM file exists and is non-empty
  - Calls `aligncount_cpp -a <path>` and writes to count stdout
### C++ Executable (aligncount_cpp)
- Source: cpp/standalone/source/main.cpp
- Build system: CMake (CMakeLists.txt)
- Functionality: Counts the number of SAM records
---
## Installation
### Build and install the Python Wrapper and the C++ tool
To install in a virtualenv while at repo root:

    python3 -m venv venv
    source venv/bin/activate
    pip install --upgrade pip setuptools scikit-build
    pip install .

Now you have:
   - aligncount_cpp  (C++ binary, in venv/bin)
   - aligncount      (Python wrapper, in venv/bin)

Run the wrapper (counts the number of SAM records in a file):

    aligncount count-mapped -a sample.sam
    aligncount count-unmapped -a sample.sam

Note, the installation command implicitly builds the C++ tool via scikit-build/cmake. If you want to 
install the Python Wrapper by itself do (does not install aligncount_cpp):
    
    python3 -m venv venv
    pip install --upgrade pip setuptools
    cd cli
    pip install .

### Dockerfile
A multi-stage Dockerfile builds and tests everything, then produces a minimal runtime image:
1. Builder stage:
   - Installs build-time dependencies (CMake, Git, Python dev headers, pip)
   - Compiles C++ (aligncount_cpp) and runs CTest
   - Builds a Python wheel (bundles aligncount_cpp + aligncount)
2. Final stage:
   - Installs runtime dependencies (Python, pip)
   - Installs the wheel (placing aligncount_cpp and aligncount into /usr/local/bin)
   - Runs the Python wrapper unit tests
   - Sets ENTRYPOINT ["aligncount"]

Build and test in one command:

    docker build -t aligncount:latest .

Run:

    docker run --rm aligncount:latest --help


### Manually build the C++ tool aligncount_cpp
To build manually while at repo root:

    mkdir build
    cd build
    cmake ../cpp/standalone -DCMAKE_BUILD_TYPE=Release
    cmake --build .

Now you have aligncount_cpp available in the build folder. Run the tool to counts SAM records:

    ./aligncount -a sample.sam

---
## Unit Tests

### C++ Tests
To run the C++ tool tests at repo root do:

    cmake -S cpp/test -B build/test
    cmake --build build/test
    CTEST_OUTPUT_ON_FAILURE=1 cmake --build build/test --target test

### Python Tests
To run the python wrapper tests at repo root do:

    export PYTHONPATH="$PWD/cli:${PYTHONPATH}"
    pytest tests

The export command is required if the wrapper is not installed.

---
## GitHub Action Workflows

### On new tag releases:
- `.github/workflows/conda_release.yml` Creates and publishes a conda package for various platforms.
- `.github/workflows/release_release.yml` Compiles and publishes the Dockerfile to docker hub.
- `.github/workflows/cpp_documentation.yml` Builds and publishes documentation for the C++ tool.
### On commits or pull requests to main/master branch:
- `.github/workflows/cpp_wrapper_ci.yml` Builds, installs, and tests the C++ tool and wrapper on ubuntu.
- `.github/workflows/cpp_install.yml` Builds, installs, and tests the C++ tool on ubuntu.
- `.github/workflows/cpp_macos.yml` Builds and tests the C++ tool on MacOS.
- `.github/workflows/cpp_standalone.yml` Builds the C++ tool on ubuntu.
- `.github/workflows/cpp_ubuntu.yml` Tests the C++ tool on ubuntu.
- `.github/workflows/cpp_style.yml` Checks C++ and CMake source style.
---
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

---
## Conda-Build Recipe

The `conda-recipe/meta.yaml` contains instructions for the conda-build command to create the conda package that 
contains the C++ tool and the python wrapper. The recipe expects certain environment variables to be set, which 
are set by the GitHub action `conda_release.yml`. The expected environment variables are:

```
REPO_NAME   # Name of the repo, e.g. cpp-python-tool-template
VERSION     # Tool version, e.g. 0.0.1
TAR_URL     # URL to the repo archive, e.g. https://github.com/user/foo/archive/refs/tags/v0.0.1.tar.gz
SHA256      # SHA256 checksum of the repo archive
REPO_HOME   # URL to the repo home, e.g. https://github.com/user/foo
```
---
## Miscellaneous Notes

---
## What’s Next

- See github issues for this repo

With this template, you have a fully tested building, packaging, and distribution pipeline out of the box. Happy coding!
