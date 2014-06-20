NAME = output

SRC = $(wildcard src/*.cpp)

HEADERS = -Iincludes/

CC = g++

FLAG = -std=c++0x

LIB = -lpthread

$(NAME):
	$(CC) $(FLAG) -o $(NAME) $(SRC) $(HEADERS) $(LIB)
