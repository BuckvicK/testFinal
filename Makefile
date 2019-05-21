NAME = cars
SRC = init_window.cpp \
		main.cpp

all: $(NAME)

run: $(NAME)
	./cars

$(NAME): $(SRC)
	clang++ -std=c++11 -I include -L lib -l SDL2-2.0.0 $(SRC) -o $(NAME)

re: clean all

clean:
	rm $(NAME)