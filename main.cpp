#include "cars.h"

std::vector<sCar*> cars;
sRect	cross;

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

bool	isCarOnCross(sCar* car){
	if (car->dir == eDirection::RIGHT){
		if ((car->getFuturePos().pos.x + car->rect.size.width >= SCREEN_WIDTH / 2 - 100) && \
			(car->getFuturePos().pos.x < SCREEN_WIDTH / 2)){
			return (true);
		}
		// printf("RIGHT\n");
	}
	else if (car->dir == eDirection::LEFT){
		if ((car->getFuturePos().pos.x <= SCREEN_WIDTH / 2 + 100) && \
			(car->getFuturePos().pos.x + car->getFuturePos().size.width > SCREEN_WIDTH / 2)){
			return (true);
		}
		// printf("LEFT\n");
	}
	else if (car->dir == eDirection::DOWN){
		if ((car->getFuturePos().pos.y + car->rect.size.height >= SCREEN_HEIGHT / 2 - 100) && \
			(car->getFuturePos().pos.y < SCREEN_HEIGHT / 2)){
			return (true);
		}
		// printf("DOWN\n");
	}
	else if (car->dir == eDirection::UP){
		if ((car->getFuturePos().pos.y <= SCREEN_HEIGHT / 2 + 100) && \
			(car->getFuturePos().pos.y + car->getFuturePos().size.height > SCREEN_HEIGHT / 2)){
			return (true);
		}
		// printf("UP\n");
	}
	return (false);
}

bool	main_loop() {
	std::vector<sCar*> moveCars;

	int	carsOnCross = 0;
	for (auto car: cars){
		if (isCarOnCross(car) == true)
			carsOnCross++;
		if (carsOnCross >= 4)
			break ;
	}

	if (carsOnCross >= 4){
		sCar* minCar = cars[0];
		for (auto car: cars){
			if (car->rect.pos.x < minCar->rect.pos.x){
				minCar = car;
			}
		}
		sRect	minCarFuturePos = minCar->getFuturePos();
		int		checkIntersect = 0;
		for (auto car: cars){
			if (car != minCar && car->rect.intersects(minCarFuturePos)){
				checkIntersect++;
				break ;
			}
		}
		if (checkIntersect == 0){
			while(isCarOnCross(minCar)){
				minCar->move();
				drawScene();
			}
		}
	}

	for (int i = 0; i < cars.size(); i++){
		int		intr = 0;
		int 	passCount = 0;
		sCar*	currentCar = cars[i];

		// if (isCarOnCross(currentCar)){
		// 	for (auto c: cars){
		// 		if (currentCar != c && isCarOnCross(c)){
		// 			if (currentCar->needPassOtherCar(c)) {
		// 				passCount++;
		// 				break ;
		// 			}
		// 		}
		// 	}
		// }
		for (int j = 0;  j < cars.size(); j++) {
			sCar*	otherCar = cars[j];
			if (currentCar != otherCar) {
				if (currentCar->getFuturePos().intersects(otherCar->getFuturePos())) {
					if (currentCar->needPassOtherCar(otherCar)) {
						passCount++;
						break ;
					}
				}
				if (currentCar->getFuturePos().intersects(otherCar->rect))
					intr++;
			}
		}
		if (passCount != 0)
			continue ;
		if (intr == 0)
			moveCars.push_back(currentCar);

		if (currentCar->rect.pos.x < -currentCar->rect.size.width || \
			currentCar->rect.pos.y < -currentCar->rect.size.height || \
			currentCar->rect.pos.x > SCREEN_WIDTH || \
			currentCar->rect.pos.y > SCREEN_HEIGHT) {
				sCar* tmp = cars[i];
				delete tmp;
				cars.erase(cars.begin() + i);
				break;
		}
	}

	/*
	for (int i = 0; i < cars.size(); i++){
		int		pass = 0;
		int		intr = 0;
		sCar*	currentCar = cars[i];

		passCount = 0;
		for (int j = 0;  j < cars.size(); j++) {
			sCar*	otherCar = cars[j];
			if (currentCar != otherCar) {
				if (currentCar->getFuturePos().intersects(otherCar->getFuturePos())) {
					if (currentCar->needPassOtherCar(otherCar)) {
						passCount++;
						break ;
					}
				}
				if (currentCar->getFuturePos().intersects(otherCar->rect))
					intr++;
			}
		}
		if (passCount != 0)
			continue ;
		if (intr == 0)
			moveCars.push_back(currentCar);

		if (currentCar->rect.pos.x < -currentCar->rect.size.width || \
			currentCar->rect.pos.y < -currentCar->rect.size.height || \
			currentCar->rect.pos.x > SCREEN_WIDTH || \
			currentCar->rect.pos.y > SCREEN_HEIGHT) {
				sCar* tmp = cars[i];
				delete tmp;
				cars.erase(cars.begin() + i);
				break;
		}
	}
	*/

	for (auto c: moveCars)
		c->move();
	moveCars.clear();

	if (cars.size() < initialCarsCount)
		spawnCar();
	return (0);
}

void	testSpawn(){
	spawnCarFromLeft();
	cars[0]->rect.pos.x = SCREEN_WIDTH / 2 - 100 - cars[0]->rect.size.width;
	spawnCarFromRight();
	cars[1]->rect.pos.x = SCREEN_WIDTH / 2 + 100;
	spawnCarFromTop();
	cars[2]->rect.pos.y = SCREEN_HEIGHT / 2 - 100 - cars[2]->rect.size.width;
	spawnCarFromBot();
	cars[3]->rect.pos.y = SCREEN_HEIGHT / 2 + 100;
}

void	createCross(){
	cross.pos.x = SCREEN_WIDTH / 2 - 100;
	cross.pos.y = SCREEN_HEIGHT / 2 - 100;
	cross.size.width = 200;
	cross.size.height = 200;
}

int		main(int argc, char** argv) {
	createCross();
	// for (auto i = 0; i < initialCarsCount; ++i) {
	// 	spawnCar();
	// }
	testSpawn();
	init_window();
	return 0;
}
