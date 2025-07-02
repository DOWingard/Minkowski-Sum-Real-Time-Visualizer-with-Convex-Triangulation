NAME ?= myprogram

CXX = g++
CXXFLAGS = -fdiagnostics-color=always -g -std=c++17
LIBS = -lGL -lGLEW -lglfw

INCLUDE_DIRS = -I./src -I../../ -I./Testing

MAIN_SRC = src/main.cpp
TRIANGULATOR_TEST_SRC = Testing/subTesting/Test_convexDecomp/test_decomp.cpp
MINKOWSKI_TEST_SRC = Testing/subTesting/Test_convexDecomp/test_minkowski.cpp



.DEFAULT_GOAL := help
main:
	@echo "Building main executable: $(NAME)"
	@if $(CXX) $(CXXFLAGS) -I./src $(MAIN_SRC) -o "$(NAME)" $(LIBS); then \
		echo "Successfully created: $(NAME)"; \
		echo "Press any key to close terminal..."; \
		read dummy; \
	else \
		echo "Build failed: Get better at C++! (Fortran never fails)"; \
		echo "Press any key to close terminal..."; \
		read dummy; \
	fi

# MINKOWSKI SUM - UNIT TEST - Triangulator test
test-triangulator:
	@echo "Building triangulator test: $(NAME)"
	@if $(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) $(TRIANGULATOR_TEST_SRC) -o "$(NAME)" $(LIBS); then \
		echo "Successfully created: $(NAME)"; \
		echo "Press any key to close terminal..."; \
		read dummy; \
	else \
		echo "Build failed: Get better at C++! (Fortran never fails)"; \
		echo "Press any key to close terminal..."; \
		read dummy; \
	fi

# MINKOWSKI SUM - UNIT TEST - Minkowski Sum
test-minkowski:
	@echo "Building Minkowski sum test: $(NAME)"
	@if $(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) $(MINKOWSKI_TEST_SRC) -o "$(NAME)" $(LIBS); then \
		echo "Successfully created: $(NAME)"; \
		echo "Press any key to close terminal..."; \
		read dummy; \
	else \
		echo "Build failed: Get better at C++! (Fortran never fails)"; \
		echo "Press any key to close terminal..."; \
		read dummy; \
	fi

# Convenience targets with default names
main-default:
	@$(MAKE) main NAME=main_program

test-triangulator-default:
	@$(MAKE) test-triangulator NAME=triangulator_test

test-minkowski-default:
	@$(MAKE) test-minkowski NAME=minkowski_test

# Clean up executables
clean:
	@echo "Cleaning up executables..."
	@rm -f main_program triangulator_test minkowski_test $(NAME)
	@echo "Clean complete."

# Help target
help:
	@echo "Minkowski Sum Project Makefile"
	@echo ""
	@echo "Usage:"
	@echo "  make <target> NAME=<executable_name>"
	@echo ""
	@echo "Targets:"
	@echo "  main                    Build main executable (specify NAME=<name>)"
	@echo "  test-triangulator       Build triangulator test (specify NAME=<name>)"
	@echo "  test-minkowski          Build Minkowski sum test (specify NAME=<name>)"
	@echo ""
	@echo "Default name targets:"
	@echo "  main-default            Build main as 'main_program'"
	@echo "  test-triangulator-default Build triangulator test as 'triangulator_test'"
	@echo "  test-minkowski-default  Build Minkowski test as 'minkowski_test'"
	@echo ""
	@echo "Utility:"
	@echo "  clean                   Remove built executables"
	@echo "  help                    Show this help message"
	@echo ""
	@echo "Examples:"
	@echo "  make main NAME=myapp"
	@echo "  make test-triangulator NAME=test1"
	@echo "  make main-default"
.PHONY: main test-triangulator test-minkowski main-default test-triangulator-default test-minkowski-default clean help