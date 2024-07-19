#include "../inc/head.h"

SDL_Rect drect;
SDL_Texture *playerTex;
SDL_Surface *tmpSurface;
bool isRunning;
uint32_t *colorBuffer;
SDL_Texture *colorBufferTexture;

/**
 * init_instance - initializing SDL2
 * @instance: new instance
 * Return: 0 if it fails, 1 if it's successful.
 */
int init_instance(Game *instance)
{
        /* Initializing SDL */
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        {
                fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
		return 0;
	}
	printf("SDL Initialized\n");

	// Create a new Renderer instance linked to the Window
	instance->renderer = SDL_CreateRenderer(
						instance->window, -1,
						SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
						);
	if (!instance->renderer)
	{
		fprintf(stderr, "Failed to create renderer: %s\n", SDL_GetError());
		SDL_DestroyWindow(instance->window);
		SDL_Quit();
		return 0;
	}
	SDL_SetRenderDrawColor(instance->renderer, 255, 255, 255, 255);
	printf("Renderer successfully created\n");

	tmpSurface = IMG_Load("assets/Player.png");
	if (!tmpSurface)
	{
		fprintf(stderr, "Failed to load image: %s\n", SDL_GetError());
		return 0;
	}
	SDL_SetColorKey(tmpSurface, SDL_TRUE, SDL_MapRGBA(tmpSurface->format, 255, 0, 255, 255));
	instance->playerTex = SDL_CreateTextureFromSurface(instance->renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);

	if (!instance->playerTex)
	{
		fprintf(stderr, "Failed to create texture: %s\n", SDL_GetError());
		return 0;
	}

	// Allocate memory for color buffer
	colorBuffer = (Uint32 *)malloc(xpos * ypos * sizeof(Uint32));
	if (!colorBuffer)
	{
		fprintf(stderr, "Failed to allocate colorBuffer\n");
		SDL_DestroyRenderer(instance->renderer);
		SDL_DestroyWindow(instance->window);
		SDL_Quit();
		return 0;
	}

	// Create the SDL texture
	colorBufferTexture = SDL_CreateTexture(
					       instance->renderer,
					       SDL_PIXELFORMAT_ARGB8888,
					       SDL_TEXTUREACCESS_STREAMING,
					       xpos,
					       ypos
					       );
	if (!colorBufferTexture)
	{
		fprintf(stderr, "Failed to create texture: %s\n", SDL_GetError());
		free(colorBuffer);
		SDL_DestroyRenderer(instance->renderer);
		SDL_DestroyWindow(instance->window);
		SDL_Quit();
		return 0;
	}

	printf("colorBuffer allocated at address: %p\n", (void*)colorBuffer);
	return 1;
}

/**
 * render_player - rendering the player
 * @instance: Game instance
 * Return: void
 */
void render_player(Game instance)
{
        SDL_Rect playerRect = {
            p.x * MINIMAP_SCALE_FACTOR, p.y * MINIMAP_SCALE_FACTOR,
            p.width * MINIMAP_SCALE_FACTOR, p.height * MINIMAP_SCALE_FACTOR};

        drect.h = 250;
        drect.w = 500;
        drect.x = 450;
        drect.y = 450;

        SDL_SetRenderDrawColor(instance.renderer, 255, 255, 255, 255);
        SDL_RenderCopy(instance.renderer, playerTex, NULL, &drect);
        SDL_RenderFillRect(instance.renderer, &playerRect);

        SDL_RenderDrawLine(
            instance.renderer, MINIMAP_SCALE_FACTOR * p.x,
            MINIMAP_SCALE_FACTOR * p.y,
            MINIMAP_SCALE_FACTOR * (p.x + cos(p.rotation_angle) * 40),
            MINIMAP_SCALE_FACTOR * (p.y + sin(p.rotation_angle) * 40));
}

/**
 * cleanUp - Frees allocated memory and quits SDL
 * @instance: Pointer to the game instance
 */
void cleanUp(Game instance)
{
	if (colorBuffer)
	{
		free(colorBuffer);
	}

	if (colorBufferTexture)
	{
		SDL_DestroyTexture(colorBufferTexture);
	}

	if (instance.playerTex)
	{
		SDL_DestroyTexture(instance.playerTex);
	}

        SDL_DestroyRenderer(instance.renderer);
        SDL_DestroyWindow(instance.window);
        SDL_Quit();
}
