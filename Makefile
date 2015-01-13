# Makefile for joos compiler
CC=g++
CFLAGS=-std=c++11 -c -Wall -I./include
BUILD_DIR=build/src
OUT_FILE=joosc

# Main Code
SRC_C = $(wildcard src/*.cpp)
SRC_O = $(addprefix build/src/, $(notdir $(SRC_C:.cpp=.o)))

# Test Code
TEST_C = $(wildcard tests/src/*.cpp)
TEST_O = $(addprefix build/tests/, $(notdir $(TEST_C:.cpp=.o)))
TEST_CFLAGS=-std=c++11 -c -Wall -I./include -I./tests/include
TEST_LIB_PATH=-L build/lib -l joos
TEST_OUT_FILE=test_joosc

# Static Lib for Tests
LIB_O = $(filter-out build/src/main.o, $(SRC_O))
JOOSLIB = build/lib/libjoos.a

.PHONY: all
all: init $(OUT_FILE) $(TEST_OUT_FILE)

.PHONY: compiler
compiler: init $(OUT_FILE)

.PHONY: tests
tests: init $(TEST_OUT_FILE)

init:
	@mkdir -p $(BUILD_DIR) build/tests build/lib

# Main Compiler
build/src/%.o: src/%.cpp
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$(notdir $(@:.cpp=.o))

$(OUT_FILE): $(SRC_O)
	$(CC) $(BUILD_DIR)/*.o -o $(OUT_FILE)

# Test Code
$(JOOSLIB): $(LIB_O)
	ar rvs $(JOOSLIB) $(LIB_O)

build/tests/%.o: tests/src/%.cpp $(JOOSLIB)
	$(CC) $(TEST_CFLAGS) $< -o build/tests/$(notdir $(@:.cpp=.o))

$(TEST_OUT_FILE): $(TEST_O)
	$(CC) $(TEST_O) $(TEST_LIB_PATH) -o $(TEST_OUT_FILE)

# Clean
.PHONY: clean
clean:
	@rm -f $(SRC_O) $(JOOSLIB) $(OUT_FILE) $(TEST_O) $(TEST_OUT_FILE)
