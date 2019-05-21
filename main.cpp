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
	std::vector<sCar*> moveCars;

	int	carOnCross = 0;
	for (auto car: cars){
		if (car->dir == eDirection::RIGHT){
			int futurePosX = car->getFuturePos().pos.x + car->rect.size.width;
			if ((futurePosX >= SCREEN_WIDTH / 2 - 100) && (futurePosX <= SCREEN_WIDTH / 2)){
				carOnCross++;
				printf("RIGHT\n");
			}
		}
		else if (car->dir == eDirection::LEFT){
			int futurePosX = car->getFuturePos().pos.x;
			if ((futurePosX <= SCREEN_WIDTH / 2 + 100) && (futurePosX >= SCREEN_WIDTH / 2)){
				carOnCross++;
				printf("LEFT\n");
			}
		}
		else if (car->dir == eDirection::DOWN){
			int futurePosY = car->getFuturePos().pos.y + car->rect.size.height;
			if ((futurePosY >= SCREEN_HEIGHT / 2 - 100) && (futurePosY <= SCREEN_HEIGHT / 2)){
				carOnCross++;
				printf("DOWN\n");
			}
		}
		else if (car->dir == eDirection::UP){
			int futurePosY = car->getFuturePos().pos.y;
			if ((futurePosY <= SCREEN_HEIGHT / 2 + 100) && (futurePosY >= SCREEN_HEIGHT / 2)){
				carOnCross++;
				printf("UP\n");
			}
		}
		if (carOnCross >= 4)
			break ;
	}
	if (carOnCross >= 4){
		printf("%d CARS ON CROSS!!!\n", carOnCross);
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
			printf("Min car move!\n");
			minCar->move();
			return (0);
		}
		getchar();
	}

	printf("Hm...\n");
	int	passCount = 0;
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
				sCar* tmp;
				tmp = cars[i];
				delete tmp;
				cars.erase(cars.begin() + i);
				break;
		}
	}

	for (auto c: moveCars)
		c->move();
	moveCars.clear();

	// if (cars.size() < initialCarsCount)
	// 	spawnCar();
	return (0);
}

void	testSpawn(){
	spawnCarFromLeft();
	cars[0]->rect.pos.x = SCREEN_WIDTH / 2 - 110 - cars[0]->rect.size.width;
	spawnCarFromRight();
	cars[1]->rect.pos.x = SCREEN_WIDTH / 2 + 110;
	spawnCarFromTop();
	cars[2]->rect.pos.y = SCREEN_HEIGHT / 2 - 110 - cars[2]->rect.size.width;
	spawnCarFromBot();
	cars[3]->rect.pos.y = SCREEN_HEIGHT / 2 + 110;
}

int		main(int argc, char** argv) {
	// for (auto i = 0; i < initialCarsCount; ++i) {
	// 	spawnCar();
	// }
	testSpawn();
	init_window();
	return 0;
}
