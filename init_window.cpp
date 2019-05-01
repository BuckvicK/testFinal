#include "cars.h"

SDL_Window* window = NULL;
SDL_Surface* screenSurface = NULL;

int		init_window(void)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		printf("SDL not init! SDL_Error: %s\n", SDL_GetError());
	else
	{
		window = SDL_CreateWindow("Test task for MYTONA", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL)
			printf("Window not create! SDL_Error: %s\n", SDL_GetError());
		else
		{
			screenSurface = SDL_GetWindowSurface(window);
			int x = 0;
			bool done = false;
			SDL_Event event;
			while (!done)
			{
				while (SDL_PollEvent(&event))
				{
					switch (event.type)
					{
						case SDL_QUIT:
							done = true;
							break;
					}
				}
				int ret = main_loop();
				std::vector<SDL_Rect> rects;
				for (int i = 0; i < cars.size(); i++) {
					SDL_Rect rect;
					rect = {cars[i]->rect.pos.x, cars[i]->rect.pos.y, cars[i]->rect.size.width, cars[i]->rect.size.height};
					rects.push_back(rect);
				}
				SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
				for (auto rect: rects) {
					SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, 0x99, 0x99, 0x99));
				}
				SDL_UpdateWindowSurface(window);
				if (ret != 0)
					done = true;
			}
		}
	}
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
