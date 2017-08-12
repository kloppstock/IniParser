LIBS=-lm
TARGET=_test

CC=gcc
CFLAGS=-Wall -Wextra -Werror -pedantic -g

SRC=$(wildcard ../src/*.c) $(wildcard *.c)
OBJ=$(SRC:.c=.o)
DEP=$(SRC:.c=.d)
TST=$(wildcard ./ini_open_and_close_file_test/*.test.txt) $(wildcard ./ini_write_section_test/*.test.txt) $(wildcard ./write_value_test/*.test.txt) $(wildcard ./ini_write_name_value_test/*.test.txt) $(wildcard ./ini_write_comment_test/*.test.txt)

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
	rm -f $(OBJ) $(TARGET) $(DEP) $(TST)

-include $(DEP)
