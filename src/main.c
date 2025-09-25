// #include <SDL3/SDL.h>
// #include <stdio.h>

// #define NAME "Project X"

// int	main()
// {
// 	SDL_Window		*window;
// 	SDL_Renderer	*renderer;
// 	SDL_Event		event;
// 	int				running;

// 	printf("Programme lancé\n");
// 	fflush(stdout);

// 	if (SDL_Init(SDL_INIT_VIDEO) != 0)
// 	{
// 		fprintf(stderr, "Erreur SDL_Init: %s\n", SDL_GetError());
// 		return (1);
// 	}
	
// 	window = SDL_CreateWindow(NAME, 800, 600, SDL_WINDOW_RESIZABLE);
// 	if (!window)
// 	{
// 		fprintf(stderr, "Erreur SDL_CreateWindow: %s\n", SDL_GetError());
// 		SDL_Quit();
// 		return (1);
// 	}

// 	renderer = SDL_CreateRenderer(window, NAME);
// 	if (!renderer)
// 	{
// 		fprintf(stderr, "Erreur SDL_CreateRenderer: %s\n", SDL_GetError());
// 		SDL_DestroyWindow(window);
// 		SDL_Quit();
// 		return (1);
// 	}

// 	running = 1;
// 	while (running)
// 	{
// 		while (SDL_PollEvent(&event))
// 		{
// 			if (event.type == SDL_EVENT_QUIT)
// 			{
// 				running = 0;
// 			}
// 		}

// 		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
// 		SDL_RenderClear(renderer);
// 		SDL_RenderPresent(renderer);
// 	}

// 	SDL_DestroyRenderer(renderer);
// 	SDL_DestroyWindow(window);
// 	SDL_Quit();
// 	return (0);
// }

#include <SDL3/SDL.h>
#include <stdio.h>

int main() { return SDL_Init(SDL_INIT_VIDEO); }
