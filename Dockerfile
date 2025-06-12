# ----------------------------------------------------------------------------
# Stage 1: Build C++ + Python wheel
# ----------------------------------------------------------------------------
FROM ubuntu:22.04 AS builder

# 1) Install build tools, CMake, Git, Python headers, and pip
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    python3-dev \
    python3-pip \
 && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# 2) Copy entire repo into /app
COPY . .

# 3) Upgrade pip and install scikit-build so that "pip wheel ." will run CMake
RUN pip3 install --upgrade pip setuptools scikit-build

# 4) Manually configure, compile, and test the C++ code
#    This builds 'Aligncount' and 'AligncountTests', then runs CTest.
RUN cd cpp && mkdir build && cd build \
 && cmake .. -DCMAKE_BUILD_TYPE=Release \
 && cmake --build . --parallel $(nproc) \
 && ctest --output-on-failure

# 4) Build a wheel into /wheelhouse
RUN pip3 wheel . -w /wheelhouse

# (At this point, /wheelhouse/ contains something like:
#  aligncount_demo-0.1.0-py3-none-any.whl )


# ----------------------------------------------------------------------------
# Stage 2: Final runtime image
# ----------------------------------------------------------------------------
FROM ubuntu:22.04

# 1) Install only runtime dependencies: Python 3
RUN apt-get update && apt-get install -y \
    python3 \
    python3-pip \
 && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# 2) Copy the wheel (using wildcard) from the builder stage
COPY --from=builder /wheelhouse/*.whl /tmp/

# 3) Upgrade pip, then install whichever .whl we copied
RUN pip3 install --upgrade pip \
 && pip3 install /tmp/*.whl \
 && rm /tmp/*.whl

# Copy only the Python unit test
COPY tests/python/test_cli.py /app/tests/python/test_cli.py

# Install pytest to run our simple unit test
RUN pip3 install pytest

# Run only the format_output() unit test
RUN pytest -q /app/tests/python/test_cli.py

# 4) Verify the binaries are on PATH (just a check; you can remove)
RUN which aligncount_cpp && which aligncount

# Final entrypoint: run the Python wrapper by default
ENTRYPOINT ["aligncount"]
CMD ["--help"]
