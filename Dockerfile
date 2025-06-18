# ----------------------------------------------------------------------------
# Stage 1: Build C++ + Python wheel
# ----------------------------------------------------------------------------
FROM ubuntu:22.04 AS builder

# Install build tools, CMake, Git, Python headers, and pip
RUN apt-get update && apt-get install -y \
    autoconf \
    build-essential \
    cmake \
    git \
    libcurl4-openssl-dev \
    python3-dev \
    python3-pip \
 && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy entire repo into /app
COPY . .

# Upgrade pip and install scikit-build so that "pip wheel ." will run CMake
RUN pip3 install --upgrade pip setuptools scikit-build

# Manually configure, compile, and test the C++ code
RUN cd cpp && mkdir build && cd build \
 && cmake ../all -DCMAKE_BUILD_TYPE=Release \
 && cmake --build . --parallel $(nproc) \
 && CTEST_OUTPUT_ON_FAILURE=1 cmake --build test --target test

# Build a wheel into /wheelhouse
RUN pip3 wheel . -w /wheelhouse

# ----------------------------------------------------------------------------
# Stage 2: Final runtime image
# ----------------------------------------------------------------------------
FROM ubuntu:22.04

# Install only runtime dependencies
RUN apt-get update && apt-get install -y \
    python3 \
    python3-pip \
    libcurl4 \
    liblzma5 \
    libbz2-1.0 \
    zlib1g \
 && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy the wheel (using wildcard) from the builder stage
COPY --from=builder /wheelhouse/*.whl /tmp/

# Upgrade pip, then install whichever .whl we copied
RUN pip3 install --upgrade pip \
 && pip3 install /tmp/*.whl \
 && rm /tmp/*.whl

# Copy only the Python unit test
COPY tests/python/test_cli.py /app/tests/python/test_cli.py

# Install pytest to run our simple unit test
RUN pip3 install pytest

# Run only the format_output() unit test
RUN pytest -q /app/tests/python/test_cli.py

# Verify the binaries are on PATH (just a check; you can remove)
RUN which aligncount_cpp && which aligncount

# Final entrypoint: run the Python wrapper by default
ENTRYPOINT ["aligncount"]
CMD ["--help"]
