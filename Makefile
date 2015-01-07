# Makefile for joos compiler
CC=g++
CFLAGS=-c -Wall -I./include
BUILD_DIR=build/src
OUT_FILE=joosc

# Main Code
SRC_C = $(wildcard src/*.cpp)
SRC_O = $(addprefix build/src/, $(notdir $(SRC_C:.cpp=.o)))

all: init $(OUT_FILE)

init:
	mkdir -p $(BUILD_DIR) build/tests

build/src/%.o: src/%.cpp
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$(notdir $(@:.cpp=.o))

$(OUT_FILE): $(SRC_O)
	$(CC) $(BUILD_DIR)/*.o -o $(OUT_FILE)

clean:
	rm -f build/src/* build/tests/*
	rm $(OUT_FILE)
