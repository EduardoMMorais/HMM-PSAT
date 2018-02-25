CC = g++
CPPFLAGS_ALL = -std=c++11 -Wall -fexceptions -Iinclude -I. -Iinclude/qsopt-ex
LDFLAGS_ALL = -lgmp -lgmpxx -lbz2 -lz -lqsopt_ex -lyices
CPPFLAGS_DEBUG = -g
LDFLAGS_DEBUG = 
CPPFLAGS_RELEASE = -O2 -s -DNDEBUG
LDFLAGS_RELEASE = 
CPPFLAGS_TEST = `pkg-config --cflags cppunit`
LDFLAGS_TEST = `pkg-config --libs cppunit`
ifeq ($(TYPE),debug)
CPPFLAGS = $(CPPFLAGS_ALL) $(CPPFLAGS_DEBUG)
LDFLAGS = $(LDFLAGS_ALL) $(LDFLAGS_DEBUG)
BIN_DIR = bin/debug
OBJ_DIR = obj/debug
else
CPPFLAGS = $(CPPFLAGS_ALL) $(CPPFLAGS_RELEASE)
LDFLAGS = $(LDFLAGS_ALL) $(LDFLAGS_RELEASE)
BIN_DIR = bin/release
OBJ_DIR = obj/release
endif
SOURCES = $(wildcard src/*.cpp)
OBJS = $(patsubst src/%,$(OBJ_DIR)/%,$(SOURCES:.cpp=.o))
TESTS = $(wildcard tests/*.cpp)
TEST_OBJS = $(patsubst tests/%,$(OBJ_DIR)/%,$(TESTS:.cpp=.o))


.PHONY : all test clean depend directories
all: directories $(BIN_DIR)/HMM-PSAT
test: directories bin/test/HMM-PSAT
	bin/test/HMM-PSAT

bin/test/HMM-PSAT: $(OBJS) $(TEST_OBJS)
	@mkdir -p bin/test
	$(CC) $(CPPFLAGS) $(CPPFLAGS_TEST) $(LDFLAGS) $(LDFLAGS_TEST) $(OBJS) $(TEST_OBJS) -o $@

$(BIN_DIR)/HMM-PSAT: $(OBJS) main.o
	$(CC) $(CPPFLAGS) $(LDFLAGS) $(OBJS) main.o -o $@

$(OBJS): $(OBJ_DIR)/%.o : src/%.cpp
	$(CC) $(CPPFLAGS) -c $< -o $@

$(TEST_OBJS): $(OBJ_DIR)/%.o : tests/%.cpp
	$(CC) $(CPPFLAGS) $(CPPFLAGS_TEST) -c $< -o $@

clean:
	rm -rvf bin obj *.lp Makefile.deps

directories:
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(OBJ_DIR)

depend:
	rm Makefile.deps
	make Makefile.deps

Makefile.deps:
	$(CC) -MM -MP $(SOURCES) $(TESTS) $(CPPFLAGS_ALL) > Makefile.deps

include Makefile.deps