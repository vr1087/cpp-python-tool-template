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


## How to use this template
- Use this repo [as a template](https://help.github.com/en/github/creating-cloning-and-archiving-repositories/creating-a-repository-from-a-template).
- Replace all occurrences of "Aligncount" with the name of your project in the relevant CMakeLists.txt under `cpp/`.
  - Capitalization matters here: `Aligncount` means the name of the project, while `aligncount` is used in file names.
  - Remember to rename the `include/aligncount` directory to use your project's lowercase name and update all relevant `#include`s accordingly.
  - Replace instances of ALIGNCOUNT_VERSION with `[YOUR_PROJECT_NAME]_VERSION`
  - Set the name of the C++ executable (e.g. aligncount_cpp) to your liking in `cpp/standalone/CMakeLists.txt`
  - Set the expected name of the C++ executable in the `cpp_standalone.yml` workflow.
  - Set the expected name of the C++ executable and python wrapper in `DockerFile`.
  - Set the name for your published docker image in `docker_release.yml`
- Replace the source files with your own
- Set GitHub action secrets for:
  - ANACONDA_TOKEN
  - ANACONDA_USER
  - DOCKERHUB_TOKEN
  - DOCKERHUB_USERNAME
  - CODECOV_TOKEN (if making code coverage reports for the C++ tool)

---
## Example Tool Descriptions
### Python Wrapper (aligncount)
- Source: cli/entrypoint.py
- Entry point: aligncount console script (configured in setup.py)
- Functionality:
  - Parses subcommands (count-mapped or count-unmapped)
  - Checks that the input SAM file exists and is non-empty
  - Calls `aligncount_cpp -a <path>` and writes to count stdout
### C++ Executable (aligncount_cpp)
- Source: cpp/standalone/source/main.cpp
- Statically links HTSlib
- Build system: CMake (CMakeLists.txt)
- Functionality: Counts the number of SAM records
---
## Installation
### Build and install the C++ tool and the Python Wrapper
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

### Manually build the C++ tool aligncount_cpp
To build manually while at repo root:

    mkdir build
    cd build
    cmake ../cpp/standalone -DCMAKE_BUILD_TYPE=Release
    cmake --build .

Now you have aligncount_cpp available in the build folder. Run the tool to counts SAM records:

    ./aligncount_cpp -a sample.sam

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
- `.github/workflows/cpp_ubuntu.yml` Tests and creates codecov reports for the C++ tool on ubuntu.
- `.github/workflows/cpp_style.yml` Checks C++ and CMake source style.
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
### Template Sources
- The development files for the C++ executable (under `cpp/`) were adapted from 
[ModernCppStarter](https://github.com/TheLartians/ModernCppStarter) after adding HTSlib support in 
a slim fork [htslib-cpp-starter](https://github.com/vr1087/htslib-cpp-starter). The contents of htslib-cpp-starter 
were added to this repo under `cpp/` via `git subtree` and further adapted.
- The implementation pattern for HTSlib support was adapted from Yang Li’s blog post: 
[Building Cpp Development in Bioinformatics](https://yangli.hashnode.dev/building-cpp-development-in-bioinformatics) 
### Conda Release
The conda release workflow publishes to your personal anacanda account. The workflow uses the repository name to name 
the conda package, which would equate to `cpp-python-tool-template` for this repo. Additionally, the workflow 
creates a conda package for the following platforms: linux-64, linux-aarch64, osx-64, and osx-arm64.

---
## What’s Next

- See github issues for this repo

With this template, you have a fully tested building, packaging, and distribution pipeline out of the box. Happy coding!
