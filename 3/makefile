SRC = home2.c daemon.c
OBJ = $(SRC:.c=.o)
CC = gcc

all: $(OBJ)
	gcc $(OBJ) -o out

%.o: %.c
	$(CC) -c $<
