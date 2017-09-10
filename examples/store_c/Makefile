LIBS=-lm
TARGET=store

CC=gcc
CFLAGS=-Wall -Wextra -Werror -O2

C_SRC=$(wildcard *.c) $(wildcard ../../src/*.c)
OBJ=$(C_SRC:.c=.o)
DEP=$(C_SRC:.c=.d)
INI=$(wildcard *.ini)

.PHONY: clean all
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $^ -o $@ $(LIBS)

%.o: %.c
	$(CC) -c -MMD $< -o $@ $(CFLAGS)

clean:
	rm -f $(OBJ) $(TARGET) $(DEP) $(INI)

-include $(DEP)
