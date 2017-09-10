LIBS=-lm
TARGET=store

CC=gcc
CXX=g++
CFLAGS=-Wall -Wextra -Werror -O2
CXXFLAGS=$(CFLAGS) --std=c++11

CXX_SRC=$(wildcard *.cpp) $(wildcard ../../src/*.cpp)
C_SRC=$(wildcard *.c) $(wildcard ../../src/*.c)
OBJ=$(C_SRC:.c=.o) $(CXX_SRC:.cpp=.o)
DEP=$(C_SRC:.c=.d) $(CXX_SRC:.cpp=.d)
INI=$(wildcard *.ini)

.PHONY: clean all
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $^ -o $@ $(LIBS)

%.o: %.c
	$(CC) -c -MMD $< -o $@ $(CFLAGS)

%.o: %.cpp
	$(CXX) -c -MMD $< -o $@ $(CXXFLAGS)

clean:
	rm -f $(OBJ) $(TARGET) $(DEP) $(INI)

-include $(DEP)
