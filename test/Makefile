LIBS=-lm
TARGET=_test

CC=clang
CXX=clang++
CFLAGS=-Wall -Wextra -Werror -g
CXXFLAGS=$(CFLAGS) --std=c++11

CXX_SRC=$(wildcard *.cpp) $(wildcard ../src/*.cpp)
C_SRC=$(wildcard *.c) $(wildcard ../src/*.c)
OBJ=$(C_SRC:.c=.o) $(CXX_SRC:.cpp=.o)
DEP=$(C_SRC:.c=.d) $(CXX_SRC:.cpp=.d)
TST=$(wildcard ./ini_open_and_close_file_test/*.test.txt) $(wildcard ./ini_write_section_test/*.test.txt) $(wildcard ./write_value_test/*.test.txt) $(wildcard ./ini_write_name_value_test/*.test.txt) $(wildcard ./ini_write_comment_test/*.test.txt) $(wildcard ./IniParser_store_test/result*)

.PHONY: clean all
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $^ -o $@ $(LIBS)

%.o: %.c
	$(CC) -c -MMD $< -o $@ $(CFLAGS)

%.o: %.cpp
	$(CXX) -c -MMD $< -o $@ $(CXXFLAGS)

test:
	make all
	./_test

clean:
	rm -f $(OBJ) $(TARGET) $(DEP) $(TST)

-include $(DEP)
