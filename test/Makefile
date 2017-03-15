LIBS=-lm
TARGET=_test

CC=gcc
CFLAGS=-Wall -Wextra -pedantic -g

SRC=$(wildcard ../src/*.c) $(wildcard *.c)
OBJ=$(SRC:.c=.o)
DEP=$(SRC:.c=.d)

.PHONY: clean all test
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $^ -o $@ $(LIBS)

%.o: %.c
	$(CC) -c -MMD $< -o $@ $(CFLAGS)

test:
	make all
	./_test

clean:
	rm -f $(OBJ) $(TARGET) $(DEP)

-include $(DEP)
