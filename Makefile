NAME = cars20
SRC = init_window.cpp \
		main.cpp
HEAD = cars.h
all: $(NAME)

run: $(NAME)
	./cars

$(NAME): $(SRC) $(HEAD)
	clang++ -std=c++11 -I include -L lib -l SDL2-2.0.0 $(SRC) -o $(NAME)

re: clean all

clean:
	rm $(NAME)