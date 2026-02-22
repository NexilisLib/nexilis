JOBS := $(patsubst -j%,%,$(filter -j%,$(MAKEFLAGS)))
ifeq ($(JOBS),)
JOBS := 1
endif

LIB_DIR      := nexilis
LIB_BUILD    := $(LIB_DIR)/build
TEST_CPP_DIR := tests/nexilis
TEST_C_DIR   := tests/nexilisc

.PHONY: all install test test-cpp test-c clean clean-tests help

all: install

install:
	@echo "Building and installing Nexilis..."
	rm -rf $(LIB_BUILD)
	cd $(LIB_DIR) && cmake -B build -DCMAKE_INSTALL_PREFIX=$(CURDIR)/$(LIB_BUILD)/install
	MAKEFLAGS= cmake --build $(LIB_BUILD) --target install --parallel $(JOBS)

test: test-cpp test-c

test-cpp:
	@echo "Building C++ tests..."
	cmake -B $(TEST_CPP_DIR)/build $(TEST_CPP_DIR)
	MAKEFLAGS= cmake --build $(TEST_CPP_DIR)/build --parallel $(JOBS)
	@echo "Running C++ tests..."
	$(TEST_CPP_DIR)/build/nexilis_tests

test-c:
	@echo "Building C tests..."
	cmake -B $(TEST_C_DIR)/build $(TEST_C_DIR)
	MAKEFLAGS= cmake --build $(TEST_C_DIR)/build --parallel $(JOBS)
	@echo "Running C tests..."
	$(TEST_C_DIR)/build/nexilis_c_tests

clean: clean-tests
	rm -rf $(LIB_BUILD)

clean-tests:
	rm -rf $(TEST_CPP_DIR)/build
	rm -rf $(TEST_C_DIR)/build

help:
	@echo "Targets:"
	@echo "  install      Build and install the library (default)"
	@echo "  test         Build and run all tests"
	@echo "  test-cpp     Build and run C++ tests only"
	@echo "  test-c       Build and run C tests only"
	@echo "  clean        Remove all build artifacts"
	@echo "  clean-tests  Remove only test build artifacts"
