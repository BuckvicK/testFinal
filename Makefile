NAME = cars
SRC = main.cpp

all: $(NAME)

run: $(NAME)
	./cars

$(NAME):
	clang++ -std=c++11 $(SRC) -o $(NAME)

re: clean all

clean:
	rm $(NAME)