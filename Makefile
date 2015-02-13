# Makefile for joos compiler
CC=g++
CFLAGS=-std=c++0x -c -Wall -I./include -I./include/dfas -I./include/weeds -I./include/AST
BUILD_DIR=build/src
OUT_FILE=joosc

# Test Folders
TEST_CASES = a1

# AST Code
AST_C = $(wildcard src/AST/*.cpp)
AST_O = $(addprefix build/src/AST/, $(notdir $(AST_C:.cpp=.o)))

# DFA Code
DFA_C = $(wildcard src/dfas/*.cpp)
DFA_O = $(addprefix build/src/dfas/, $(notdir $(DFA_C:.cpp=.o)))

# Main Code
SRC_C = $(wildcard src/*.cpp)
SRC_O = $(addprefix build/src/, $(notdir $(SRC_C:.cpp=.o))) $(DFA_O) $(AST_O)

# Test Code
TEST_C = $(wildcard tests/src/*.cpp)
TEST_O = $(addprefix build/tests/, $(notdir $(TEST_C:.cpp=.o)))
TEST_CFLAGS=-std=c++0x -c -Wall -I./include -I./tests/include -I./include/dfas -I./include/weeds -I./include/AST
TEST_LIB_PATH=-L build/lib -l joos
TEST_OUT_FILE=test_joosc

# Include
SRC_INC = $(wildcard include/*.h) $(wildcard include/dfas/*.h) $(wildcard include/weeds/*.h) $(wildcard include/AST/*.h)
TEST_INC = $(wildcard tests/include/*.h)

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
	@mkdir -p $(BUILD_DIR) $(BUILD_DIR)/dfas build/tests build/lib
	@$(foreach TEST_CASE, $(TEST_CASES), @python Extras/Scripts/listTestFiles.py tests/$(TEST_CASE)/ tests/include/$(TEST_CASE)TestFiles.h  tests/src/$(TEST_CASE)TestFiles.cpp $(TEST_CASE);)
	# @python Extras/Scripts/readParserRulesAndTable.py Extras/Grammar/grammarAndParseTable.txt include/parserRules.h src/parserRules.cpp include/parserActions.h src/parserActions.cpp

# Main Compiler
build/src/AST/%.o: src/AST/%.cpp $(SRC_INC)
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/AST/$(notdir $(@:.cpp=.o))

build/src/dfas/%.o: src/dfas/%.cpp $(SRC_INC)
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/dfas/$(notdir $(@:.cpp=.o))

build/src/%.o: src/%.cpp $(SRC_INC)
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$(notdir $(@:.cpp=.o))

$(OUT_FILE): $(SRC_O)
	$(CC) $^ -o $(OUT_FILE)

# Test Code
$(JOOSLIB): $(LIB_O)
	ar rvs $(JOOSLIB) $(LIB_O)

build/tests/%.o: tests/src/%.cpp $(JOOSLIB) $(TEST_INC)
	$(CC) $(TEST_CFLAGS) $< -o build/tests/$(notdir $(@:.cpp=.o))

$(TEST_OUT_FILE): $(TEST_O)
	$(CC) $(TEST_O) $(TEST_LIB_PATH) -o $(TEST_OUT_FILE)

# Clean
.PHONY: clean
clean:
	@rm -f $(SRC_O) $(JOOSLIB) $(OUT_FILE) $(TEST_O) $(TEST_OUT_FILE)

#debug print
print-%  : ; @echo $* = $($*)
