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
	car->speed = 10;
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
	switch (car->dir)
	{
		case (eDirection::RIGHT):
			if ((car->rect.pos.x + car->rect.size.width <= SCREEN_WIDTH / 2 - 100) && \
				(car->getFuturePos().pos.x + car->rect.size.width > SCREEN_WIDTH / 2 - 100))
				return (true);
			break;
		case (eDirection::LEFT):
			if ((car->rect.pos.x >= SCREEN_WIDTH / 2 + 100) && \
				(car->getFuturePos().pos.x < SCREEN_WIDTH / 2 + 100))
				return (true);
			break;
		case (eDirection::DOWN):
			if ((car->rect.pos.y + car->rect.size.height <= SCREEN_HEIGHT / 2 - 100) && \
				(car->getFuturePos().pos.y + car->rect.size.height > SCREEN_HEIGHT / 2 - 100))
				return (true);
			break;
		case (eDirection::UP):
			if ((car->rect.pos.y >= SCREEN_HEIGHT / 2 + 100) && \
				(car->getFuturePos().pos.y < SCREEN_HEIGHT / 2 + 100))
				return (true);
			break;
		default:
			break;
	}
	return (false);
}

struct	compareX{
	inline bool operator() (const sCar* c1, const sCar* c2){
        return (c1->rect.pos.x < c2->rect.pos.x);
    }
};

bool	main_loop() {
	std::vector<sCar*> moveCars;

	int	carsOnCross = 0;
	std::sort(cars.begin(), cars.end(), compareX());
	for (auto car: cars){
		if (isCarOnCross(car) == true)
			carsOnCross++;
		if (carsOnCross >= 4)
			break ;
	}

	for (int i = 0; i < cars.size(); i++)
	{
		sCar*	currentCar = cars[i];
		int		checkIntersect = 0;
		int		checkPass = 0;
		if (isCarOnCross(currentCar))
		{
			if (carsOnCross >= 4){
				for (int j = 0; j < cars.size(); j++)
				{
					sCar* otherCar = cars[j];
					if (currentCar != otherCar){
						if (currentCar->getFuturePos().intersects(otherCar->rect)){
							checkIntersect++;
							break ;
						}
					}
				}
				if (checkIntersect == 0){
					moveCars.push_back(currentCar);
					checkIntersect = 10;
					break ;
				}
			}
			else
			{
				for (int j = 0; j < cars.size(); j++)
				{
					sCar* otherCar = cars[j];
					if (currentCar != otherCar){
						if (currentCar->getFuturePos().intersects(otherCar->getFuturePos())){
							checkIntersect++;
							break ;
						}
						if (currentCar->needPassOtherCar(otherCar)){
							checkPass++;
							break ;
						}
					}
				}
			}
		}
		for (int j = 0; j < cars.size(); j++){
			sCar* otherCar = cars[j];
			if (currentCar != otherCar){
				if (currentCar->getFuturePos().intersects(otherCar->rect)){
					checkIntersect++;
					break ;
				}
			}
		}

		if (checkPass != 0)
			continue;
		if (checkIntersect == 0)
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

	for (auto c: moveCars)
		c->move();
	moveCars.clear();

	if (cars.size() < initialCarsCount)
		spawnCar();

	return (0);
}

int		main(int argc, char** argv) {
	for (auto i = 0; i < initialCarsCount; ++i) {
		spawnCar();
	}
	init_window();
	return 0;
}
