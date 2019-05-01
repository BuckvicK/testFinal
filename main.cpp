#include "cars.h"

std::vector<sCar*> cars;

void	spawnCar() {
	int		r;

	r = rand();
	if (r % 4 == 0)
		spawnCarFromRight();
	else if (r % 4 == 1)
		spawnCarFromTop();
	else if (r % 4 == 2)
		spawnCarFromBot();
	else if (r % 4 == 3)
		spawnCarFromLeft();
}

sCar*	randTypeCar() {
	sCar*	car;
	int		carType = rand();

	if (carType % 3 == 0)
		car = new sGasEngine();
	else if (carType % 3 == 1)
		car = new sElectroCar();
	else
		car = new sHybrid();
	car->refill(100);
	car->speed = 1;
	cars.push_back(car);
	return car;
}

void	spawnCarFromTop() {
	int	y = 100;
	for (auto c: cars){
		if (c->dir == eDirection::DOWN && c->rect.pos.y <= y)
			return ;
	}
	sCar* car = randTypeCar();

	car->rect = sRect(SCREEN_WIDTH / 2 - 100, 0, 100, 100);
	car->dir = eDirection::DOWN;
}

void	spawnCarFromBot() {
	int	y = SCREEN_HEIGHT - 100;
	for (auto c: cars){
		if (c->dir == eDirection::UP && c->rect.pos.y + c->rect.size.height >= y)
			return ;
	}
	sCar* car = randTypeCar();

	car->rect = sRect(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 100, 100, 100);
	car->dir = eDirection::UP;
}

void	spawnCarFromLeft() {
	int	x = 100;
	for (auto c: cars){
		if (c->dir == eDirection::RIGHT && c->rect.pos.x <= x)
			return ;
	}
	sCar* car = randTypeCar();

	car->rect = sRect(0, SCREEN_HEIGHT / 2, 100, 100);
	car->dir = eDirection::RIGHT;
}

void	spawnCarFromRight() {
	int	x = SCREEN_WIDTH - 100;
	for (auto c: cars){
		if (c->dir == eDirection::LEFT && c->rect.pos.x + c->rect.size.width >= x)
			return ;
	}
	sCar* car = randTypeCar();

	car->rect = sRect(SCREEN_WIDTH - 100, SCREEN_HEIGHT / 2 - 100, 100, 100);
	car->dir = eDirection::LEFT;
}

bool	main_loop() {
	std::vector<sCar*> moveCar;

	while (true) {
		int	passCount = 0;
		for (int i = 0; i < cars.size(); i++){
			int		pass = 0;
			int		intr = 0;
			sCar*	car = cars[i];
			
			passCount = 0;
			for (int j = 0;  j < cars.size(); j++) {
				sCar*	car22 = cars[j];
				if (car != car22) {
					if (car->getFuturePos().intersects(car22->getFuturePos())) {
						if (car->needPassOtherCar(car22)) {
							passCount++;
							break ;
						}
					}
					if (car->getFuturePos().intersects(car22->rect))
						intr++;
				}
			}
			if (passCount != 0)
				continue ;
			if (intr == 0)
				moveCar.push_back(car);
			if (car->rect.pos.x < -car->rect.size.width || \
				car->rect.pos.y < -car->rect.size.height || \
				car->rect.pos.x > SCREEN_WIDTH || \
				car->rect.pos.y > SCREEN_HEIGHT) {
					spawnCar();
					cars.erase(cars.begin() + i);
					break;
			}
		}
		for (auto c: moveCar)
		{
			c->move();
		}
		if (moveCar.size() == 0){
			sCar* minCar = cars[0];
			for (auto c: cars){
				if (c != minCar && c->rect.pos.x <= minCar->rect.pos.x)
					for (auto c1: cars) {
						if (c != c1 && c1 != minCar && !c->getFuturePos().intersects(c1->rect)){
							minCar = c;
						}
					}
			}
			int check = 0;
			for (auto c: cars){
				if (minCar->getFuturePos().intersects(c->rect))
					check++;
			}
			if (check == 0)
				minCar->move();
		}
		moveCar.clear();
		if (cars.size() < initialCarsCount)
			spawnCar();
	}
	return (0);
}

int		main(int argc, char** argv) {
	for (auto i = 0; i < initialCarsCount; ++i) {
		spawnCar();
	}
	main_loop();
	return 0;
}
