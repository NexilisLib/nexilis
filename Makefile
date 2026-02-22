JOBS := $(patsubst -j%,%,$(filter -j%,$(MAKEFLAGS)))
ifeq ($(JOBS),)
JOBS := 1
endif

LIB_DIR        := nexilis
LIB_BUILD      := $(LIB_DIR)/build
TEST_CPP_DIR   := tests/nexilis
TEST_C_DIR     := tests/nexilisc
TEST_CS_DIR    := tests/CSharpBindings.Tests
PRE_COMMIT_DIR := scripts/pre_commit

CSHARP_OUTPUT ?= $(CURDIR)/dist

.PHONY: all install install-csharp test test-cpp test-c test-csharp pre-commit pre-commit-all clean clean-tests help

all: install

install:
	@echo "Building and installing Nexilis..."
	rm -rf $(LIB_BUILD)
	cd $(LIB_DIR) && cmake -B build -DCMAKE_INSTALL_PREFIX=$(CURDIR)/$(LIB_BUILD)/install
	MAKEFLAGS= cmake --build $(LIB_BUILD) --target install --parallel $(JOBS)

install-csharp:
	@echo "Building and installing C# bindings..."
	python3 scripts/install_csharp.py --input $(CURDIR)/$(LIB_DIR) --output $(CSHARP_OUTPUT)

test: test-cpp test-c test-csharp

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

test-csharp:
	@echo "Building C# bindings..."
	cd bindings/csharp && dotnet build
	@echo "Running C# tests..."
	cd $(TEST_CS_DIR) && dotnet test

pre-commit:
	cd $(PRE_COMMIT_DIR) && python3 pre_commit.py --minimal

pre-commit-all:
	cd $(PRE_COMMIT_DIR) && python3 pre_commit.py --all

clean: clean-tests
	rm -rf $(LIB_BUILD)

clean-tests:
	rm -rf $(TEST_CPP_DIR)/build
	rm -rf $(TEST_C_DIR)/build

help:
	@echo "Targets:"
	@echo "  install          Build and install the library (default)"
	@echo "  install-csharp   Build and deploy C# bindings (CSHARP_OUTPUT=./dist)"
	@echo "  test             Build and run all tests"
	@echo "  test-cpp         Build and run C++ tests only"
	@echo "  test-c           Build and run C tests only"
	@echo "  test-csharp      Build and run C# tests only"
	@echo "  pre-commit       Run minimal pre-commit checks (format, best practices, cppcheck)"
	@echo "  pre-commit-all   Run all pre-commit checks including tests"
	@echo "  clean            Remove all build artifacts"
	@echo "  clean-tests      Remove only test build artifacts"
