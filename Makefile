OBJS = *.c

CC = gcc

COMPILER_FLAGS = -Wall -Werror -std=c99 -pedantic

OBJ_NAME = tp

all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
