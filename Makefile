###############################################################################
# PrototypeSTL - Build System
#
# Usage:
#   make            - Build all (tests, examples, benchmarks)
#   make tests      - Build and run test suite
#   make examples   - Build examples
#   make benchmarks - Build benchmarks
#   make docs       - Serve documentation locally
#   make clean      - Remove build artifacts
#
###############################################################################

CXX       ?= g++
CXXSTD    ?= c++11
CXXFLAGS  := -std=$(CXXSTD) -fno-exceptions -fno-rtti -Wall -Wextra -Wpedantic
CXXFLAGS  += -Iinclude -I.

# Release/Debug
ifdef DEBUG
  CXXFLAGS += -g -O0 -DPROTOTYPE_DEBUG=1
else
  CXXFLAGS += -O2 -DNDEBUG
endif

# Output directory
BUILDDIR  := .dist

# Sources
TEST_SRC  := tests/test_all.cpp
EXAMPLE_SRCS := examples/hft_orderbook.cpp examples/automotive_ecu.cpp examples/embedded_sensor.cpp
BENCH_SRCS := benchmarks/bench_vector.cpp

# Targets
TEST_BIN  := $(BUILDDIR)/test_all
EXAMPLE_BINS := $(BUILDDIR)/hft_orderbook $(BUILDDIR)/automotive_ecu $(BUILDDIR)/embedded_sensor
BENCH_BINS := $(BUILDDIR)/bench_vector

.PHONY: all tests examples benchmarks clean run_tests docs check_all_standards

all: tests examples benchmarks

# Create build dir
$(BUILDDIR):
	mkdir -p $(BUILDDIR)

# Tests
tests: $(TEST_BIN)
	@echo "=== Running Tests ==="
	./$(TEST_BIN)

$(TEST_BIN): $(TEST_SRC) | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -o $@ $<

# Examples
examples: $(EXAMPLE_BINS)

$(BUILDDIR)/hft_orderbook: examples/hft_orderbook.cpp | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -o $@ $<

$(BUILDDIR)/automotive_ecu: examples/automotive_ecu.cpp | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -o $@ $<

$(BUILDDIR)/embedded_sensor: examples/embedded_sensor.cpp | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -o $@ $<

# Benchmarks
benchmarks: $(BENCH_BINS)

$(BUILDDIR)/bench_vector: benchmarks/bench_vector.cpp | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -O3 -o $@ $<

# Documentation
docs:
	@echo "=== Serving Documentation ==="
	mkdocs serve

docs-build:
	mkdocs build

# Run everything
run_tests: tests
	@echo ""
	@echo "=== Running Examples ==="
	./$(BUILDDIR)/hft_orderbook
	@echo ""
	./$(BUILDDIR)/automotive_ecu
	@echo ""
	./$(BUILDDIR)/embedded_sensor

# Clean
clean:
	rm -rf $(BUILDDIR)

# Standard compliance checks
check_cpp11:
	$(CXX) -std=c++11 -fno-exceptions -fno-rtti -Iinclude -I. -fsyntax-only tests/test_all.cpp
	@echo "C++11 compliance: OK"

check_cpp14:
	$(CXX) -std=c++14 -fno-exceptions -fno-rtti -Iinclude -I. -fsyntax-only tests/test_all.cpp
	@echo "C++14 compliance: OK"

check_cpp17:
	$(CXX) -std=c++17 -fno-exceptions -fno-rtti -Iinclude -I. -fsyntax-only tests/test_all.cpp
	@echo "C++17 compliance: OK"

check_all_standards: check_cpp11 check_cpp14 check_cpp17
	@echo "All standards pass!"
