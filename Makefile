LIBS=
TARGET=ini

CC=gcc
CPP=g++
CFLAGS=-Wall -Wextra -Werror
CPPFLAGS=$(CFLAGS)

CPP_SRC=$(wildcard *.cpp) $(wildcard src/*.cpp)
C_SRC=$(wildcard *.c) $(wildcard src/*.c)
OBJ=$(C_SRC:.c=.o) $(CPP_SRC:.cpp=.o)
DEP=$(C_SRC:.c=.d) $(CPP_SRC:.cpp=.d)

.PHONY: clean all
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CPP) $^ -o $@ $(LIBS)

%.o: %.c
	$(CC) -c -MMD $< -o $@ $(CFLAGS)

%.o: %.cpp
	$(CPP) -c -MMD $< -o $@ $(CPPFLAGS)

clean:
	rm -f $(OBJ) $(TARGET) $(DEP)

-include $(DEP)
