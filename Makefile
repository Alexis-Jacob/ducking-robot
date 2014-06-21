NAME = output

SRC = $(wildcard src/*.cpp)

HEADERS = -Iincludes/

CC = g++

FLAG = -std=c++0x

$(NAME):
	$(CC) $(FLAG) -o $(NAME) $(SRC) $(HEADERS)
