#include "macfile.h"

std::vector<sCar*> cars;
void	draw_cars(); 
void	init_SDL();

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
	// printf("Spawn to go DOWN\n"); 
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
	// printf("Spawn to go UP\n");
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
	// printf("Spawn to go RIGHT\n");
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
	// printf("Spawn to go LEFT\n");
}

bool	main_loop() {
	std::vector<sCar*> moveCar;

	// while (true) {
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
						printf("CAR[%d]: x = %d; y = %d;\n", i, car->rect.pos.x, car->rect.pos.y);
						printf("CAR[%d]: x = %d; y = %d;\n", j, car22->rect.pos.x, car22->rect.pos.y);
						printf("NEED_PASS: %d\n", car->needPassOtherCar(car22));
						if (car->needPassOtherCar(car22)) {
							passCount++;
							printf("CAR_PASS: %d\n", i);
							break ;
						}
					}
					if (car->getFuturePos().intersects(car22->rect))
						intr++;
				}
			}
			printf("PASS_COUNT: %d\n", passCount);
			if (passCount != 0)
				continue ;
			if (intr == 0)
				moveCar.push_back(car);
			printf("INTR: %d CAR:%d\n", intr, i);
			if (car->rect.pos.x < -car->rect.size.width || \
				car->rect.pos.y < -car->rect.size.height || \
				car->rect.pos.x > SCREEN_WIDTH || \
				car->rect.pos.y > SCREEN_HEIGHT) {
					// spawnCarFromTop();
					spawnCar();
					cars.erase(cars.begin() + i);
					break;
			}
		}
		/*
		for (auto c: moveCar)
		{
			if (c->dir == eDirection::UP)
				printf("Car_DIR: UP\n");
			if (c->dir == eDirection::LEFT)
				printf("Car_DIR: LEFT\n");
			if (c->dir == eDirection::DOWN)
				printf("Car_DIR: DOWN\n");
			if (c->dir == eDirection::RIGHT)
				printf("Car_DIR: RIGHT\n");
		}
		*/
		for (auto c: moveCar)
		{
			c->move();
		}
		printf("MOVE_LEN: %lu\n", moveCar.size());
		// if (moveCar.size() == 0)
		// 	return 0;
		// moveCar.clear();
		printf("==================\n");
		for (int k = 0; k < cars.size(); k++)
		{
			sCar* c = cars[k];
			if (c->dir == eDirection::UP)
				printf("Car_DIR: UP\n");
			if (c->dir == eDirection::LEFT)
				printf("Car_DIR: LEFT\n");
			if (c->dir == eDirection::DOWN)
				printf("Car_DIR: DOWN\n");
			if (c->dir == eDirection::RIGHT)
				printf("Car_DIR: RIGHT\n");
			printf("CAR[%d]: x = %d; y = %d;\n", k, c->rect.pos.x, c->rect.pos.y);
			printf("\n");
		}
		printf("==================\n");
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
		for (int i = 0; i < cars.size(); i++) {
			for (int j = 0; j < cars.size(); j++) {
				if (cars[i] != cars[j] && cars[i]->rect.intersects(cars[j]->rect))
				{
					printf("%d\t%d\tAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n", i, j);
					return 1;
				}
			}
		}
		if (cars.size() < initialCarsCount)
			spawnCar();
		// draw_cars();
		// getchar();
	// }
	return (0);
}

void	test_spawn() {

	// spawnCarFromLeft();
	// cars[0]->rect.pos.x = 100 * 5 + 20;
	// spawnCarFromLeft();
	// cars[0]->rect.pos.x = 100 + 20;
	// spawnCarFromBot();
	// spawnCarFromTop();
	// spawnCarFromRight();

	// spawnCarFromRight();
	// cars[0]->rect.pos.x = 334 + 20;
	// spawnCarFromLeft();
	// cars[1]->rect.pos.x = 410;
	// cars[1]->rect.pos.y = 384;
	// spawnCarFromTop();
	// cars[1]->rect.pos.x = 412;
	// cars[1]->rect.pos.y = 180 - 20;
	// printf("PASS: %d\n", cars[0]->needPassOtherCar(cars[1]));

	spawnCarFromTop();
	cars[0]->rect.pos.y = SCREEN_HEIGHT - 100 * 1;
	spawnCarFromTop();
	cars[1]->rect.pos.y = SCREEN_HEIGHT - 100 * 2;
	spawnCarFromTop();
	cars[2]->rect.pos.y = SCREEN_HEIGHT - 100 * 3;
	spawnCarFromTop();
	cars[3]->rect.pos.y = SCREEN_HEIGHT - 100 * 4;
	spawnCarFromTop();
	cars[4]->rect.pos.y = SCREEN_HEIGHT - 100 * 5;
	spawnCarFromTop();
	cars[5]->rect.pos.y = SCREEN_HEIGHT - 100 * 6;
	spawnCarFromTop();
	cars[6]->rect.pos.y = SCREEN_HEIGHT - 100 * 7;
	spawnCarFromLeft();

	/*
	spawnCarFromLeft();
	cars[0]->rect.pos.x = SCREEN_WIDTH - 100 * 1;
	spawnCarFromLeft();
	cars[1]->rect.pos.x = SCREEN_WIDTH - 100 * 2;
	spawnCarFromLeft();
	cars[2]->rect.pos.x = SCREEN_WIDTH - 100 * 3;
	spawnCarFromLeft();
	cars[3]->rect.pos.x = SCREEN_WIDTH - 100 * 4;
	spawnCarFromLeft();
	cars[4]->rect.pos.x = SCREEN_WIDTH - 100 * 5;
	spawnCarFromLeft();
	cars[5]->rect.pos.x = SCREEN_WIDTH - 100 * 6;
	spawnCarFromLeft();
	cars[6]->rect.pos.x = SCREEN_WIDTH - 100 * 7;
	spawnCarFromLeft();
	cars[7]->rect.pos.x = SCREEN_WIDTH - 100 * 8;
	spawnCarFromLeft();
	cars[8]->rect.pos.x = SCREEN_WIDTH - 100 * 9;
	spawnCarFromBot();
	*/

	/*
	spawnCarFromBot();
	cars[0]->rect.pos.y = SCREEN_HEIGHT - 100 * 9;
	spawnCarFromBot();
	cars[1]->rect.pos.y = SCREEN_HEIGHT - 100 * 8;
	spawnCarFromBot();
	cars[2]->rect.pos.y = SCREEN_HEIGHT - 100 * 7;
	spawnCarFromBot();
	cars[3]->rect.pos.y = SCREEN_HEIGHT - 100 * 6;
	spawnCarFromBot();
	cars[4]->rect.pos.y = SCREEN_HEIGHT - 100 * 5;
	spawnCarFromBot();
	cars[5]->rect.pos.y = SCREEN_HEIGHT - 100 * 4;
	spawnCarFromBot();
	cars[6]->rect.pos.y = SCREEN_HEIGHT - 100 * 3;
	spawnCarFromBot();
	cars[7]->rect.pos.y = SCREEN_HEIGHT - 100 * 2;
	spawnCarFromRight();
	*/
	
	/*
	spawnCarFromTop();
	spawnCarFromRight();
	cars[1]->rect.pos.x = SCREEN_WIDTH - 100 * 10;
	spawnCarFromRight();
	cars[2]->rect.pos.x	= SCREEN_WIDTH - 100 * 9;
	spawnCarFromRight();
	cars[3]->rect.pos.x	= SCREEN_WIDTH - 100 * 8;
	spawnCarFromRight();
	cars[4]->rect.pos.x	= SCREEN_WIDTH - 100 * 7;
	spawnCarFromRight();
	cars[5]->rect.pos.x	= SCREEN_WIDTH - 100 * 6;
	spawnCarFromRight();
	cars[6]->rect.pos.x	= SCREEN_WIDTH - 100 * 5;
	spawnCarFromRight();
	cars[7]->rect.pos.x	= SCREEN_WIDTH - 100 * 4;
	spawnCarFromRight();
	cars[8]->rect.pos.x	= SCREEN_WIDTH - 100 * 3;
	spawnCarFromRight();
	*/

	// spawnCarFromRight();
	// cars[9]->rect.pos.x	= SCREEN_WIDTH - 100 * 2;
	// spawnCarFromRight();
	// cars[10]->rect.pos.x = SCREEN_WIDTH - 100 * 1;

	// spawnCarFromBot();
	// cars[3]->rect.pos.x = 512;
	// cars[1]->rect.pos.y = 384 + 5;

	// spawnCarFromBot();
	// cars[0]->rect.pos.y = 348;
	// spawnCarFromRight();
	// cars[1]->rect.pos.x = 614;
	// printf("CAR[0]: x = %d; y = %d;\n", cars[0]->rect.pos.x, cars[0]->rect.pos.y);
	// printf("CAR[1]: x = %d; y = %d;\n", cars[1]->rect.pos.x, cars[1]->rect.pos.y);
	// printf("PASS: %d\n", cars[0]->needPassOtherCar(cars[1]));
	// getchar();
}

/*
void test() {
	sCar* t1 = new sGasEngine();
	sCar* t2 = new sGasEngine();
	sCar* t3 = new sGasEngine();
	sCar* t4 = NULL;
	cars.push_back(t1);
	cars.push_back(t2);
	cars.push_back(t3);
	for (auto c : cars) {
		printf("1\n");
	}
	delete t1;
}
*/

int		main(int argc, char** argv) {
	for (auto i = 0; i < initialCarsCount; ++i) {
		spawnCar();
	}
	// test_spawn();
	init_window();
	// main_loop();
	return 0;
}

/*
sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Cars!");
*/

/*
void	draw_cars() 
{
	std::vector<sf::RectangleShape*> rects;
	window.setKeyRepeatEnabled(false);

	for (int i = 0; i < cars.size() && i < 20; i++) {
		sf::RectangleShape* rect = new sf::RectangleShape;
		rect->setSize(sf::Vector2f(cars[i]->rect.size.width, cars[i]->rect.size.height));
		rect->setFillColor(sf::Color(255, 0, 0, 255));
		rect->setPosition(sf::Vector2f(cars[i]->rect.pos.x, cars[i]->rect.pos.y));
		rects.push_back(rect);
	}
	if (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed &&  event.key.code == sf::Keyboard::Up)
				printf("UP!");
		}
		window.clear();
		for (auto rect : rects) {
			window.draw(*rect);
		}
		window.display();
		Sleep(2);
	}
}
*/