all:
	clang++ -g -std=c++11 -I include -L lib -l SDL2-2.0.0 main.cpp macfile.cpp