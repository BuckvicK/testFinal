NAME = cars
SRC = main.cpp

all: $(NAME)

run: $(NAME)
	./cars

$(NAME):
	clang++ -std=c++11 -I include -L lib -l SDL2-2.0.0 $(SRC) -o $(NAME)

re: clean all

clean:
	rm $(NAME)