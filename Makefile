NAME = cars
SRC = main.cpp
HEAD = cars.h

all: $(NAME)

run: $(NAME)
	./cars

$(NAME): $(SRC) $(HEAD)
	clang++ -std=c++11 $(SRC) -o $(NAME)

re: clean all

clean:
	rm $(NAME)