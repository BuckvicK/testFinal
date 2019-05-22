#ifndef CARS_H
# define CARS_H
#include <vector>
#include <stdio.h>
#include <SDL2/SDL.h>

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

struct sPos {
	int x;
	int y;
	sPos() { x = 0; y = 0; }
	sPos(int aX, int aY) { x = aX; y = aY; }
};

struct sSize
{
	int width;
	int height;
	sSize() { width = 0; height = 0; }
	sSize(int aW, int aH) { width = aW; height = aW; }
};

struct sRect
{
	sPos pos;
	sSize size;
	sRect() { pos.x = 0; pos.y = 0; size.width = 0; size.height = 0; };
	sRect(int x, int y, int w, int h) { pos.x = x; pos.y = y; size.width = w; size.height = h; }
	bool intersects(const sRect& other) {
			return !((other.pos.x + other.size.width <= pos.x) ||
					(other.pos.y + other.size.height <= pos.y) ||
					(other.pos.x >= pos.x + size.width) ||
					(other.pos.y >= pos.y + size.height));
	}
};

enum class eDirection {
	UP,
	LEFT,
	RIGHT,
	DOWN
};

struct	sCar {
	sRect rect;
	eDirection dir;
	int speed;

	sCar() { speed = 0; dir = eDirection::UP; };
	virtual ~sCar() {};
	void	move() {
		switch (dir) {
		case eDirection::UP:
			rect.pos.y -= speed;
			break;
		case eDirection::DOWN:
			rect.pos.y += speed;
			break;
		case eDirection::RIGHT:
			rect.pos.x += speed;
			break;
		case eDirection::LEFT:
			rect.pos.x -= speed;
			break;
		}
	}

	sRect	getFuturePos() {
		switch (dir) {
		case eDirection::UP:
			return sRect(rect.pos.x, rect.pos.y - speed, rect.size.width, rect.size.height);
		case eDirection::DOWN:
			return sRect(rect.pos.x, rect.pos.y + speed, rect.size.width, rect.size.height);
		case eDirection::RIGHT:
			return sRect(rect.pos.x + speed, rect.pos.y, rect.size.width, rect.size.height);
		case eDirection::LEFT:
			return sRect(rect.pos.x - speed, rect.pos.y, rect.size.width, rect.size.height);
		default:
			return rect;
		}
	}

	bool	needPassOtherCar(sCar* otherCar) {
		bool result = false;
		eDirection	otherdir = otherCar->dir;
		sRect		futureRect = otherCar->getFuturePos();
		switch (dir) {
			case eDirection::UP:
				if ((otherdir == eDirection::LEFT) && \
					(futureRect.pos.x <= rect.pos.x + rect.size.width) && \
					(futureRect.pos.x + futureRect.size.width >= rect.pos.x))
					result = true;
				break;
			case eDirection::DOWN:
				if ((otherdir == eDirection::RIGHT) && 
					(futureRect.pos.x + futureRect.size.width >= rect.pos.x) && \
					(futureRect.pos.x <= rect.pos.x + rect.size.width))
					result = true;
				break;
			case eDirection::RIGHT:
				if ((otherdir == eDirection::UP) && \
					(futureRect.pos.y <= rect.pos.y + rect.size.height) && \
					(futureRect.pos.y + futureRect.size.height >= rect.pos.y))
					result = true;
				break;
			case eDirection::LEFT:
				if ((otherdir == eDirection::DOWN) && \
					(futureRect.pos.y + futureRect.size.height >= rect.pos.y) && \
					(futureRect.pos.y <= rect.pos.y + rect.size.height))
					result = true;
				break;
		}
		return result;
	}

	virtual int getFuel() = 0;
	virtual void refill(int count) = 0;
};

struct sGasEngine : virtual sCar {
	int fuel;

	sGasEngine() { fuel = 0; };
	int getFuel() { return fuel; }
	void refill(int count) { fuel += count; }
	void move() { fuel--; sCar::move(); }
};

struct sElectroCar : virtual sCar {
	int charge;

	sElectroCar() { charge = 0; };
	int getFuel() { return charge; }
	void refill(int count) { charge += count; }
	void move() { charge--; sCar::move(); }
};

struct sHybrid : sGasEngine, sElectroCar {
	void refill(int count) { charge += count / 2; fuel += count / 2; }
	int getFuel() { return charge + fuel; }
	void move() {
		if ((rand() % 2 == 0))
			charge--;
		else
			fuel--;
		sCar::move();
	}
};

extern std::vector<sCar*> cars;
const int initialCarsCount = 10;

void	spawnCar();
sCar*	randTypeCar();
void	spawnCarFromTop();
void	spawnCarFromBot();
void	spawnCarFromLeft();
void	spawnCarFromRight();
bool	main_loop();

int		init_window(void);
void	drawScene();

#endif
