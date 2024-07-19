#include "../inc/head.h"

const int map[MAP_HEIGHT][MAP_WIDTH] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1},
    {1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 3, 0, 1, 0, 1},
    {1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1},
    {1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1},
    {1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1},
    {1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1},
    {1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1},
    {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

/**
 * main - Entry point of the maze game.
 * @argc: Argument count.
 * @argv: Argument vector.
 * Return: 0 on success, 1 on error.
 */
int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;
        Game instance;
        int frameTime;
        float deltaTime;
        /* Uint32 ticksLastFrame = 0; */
        int ticksLastFrame = SDL_GetTicks();
	/*  uint32_t *colorBuffer; */

	if (init_instance(&instance) != 0)
                return (1);

        /** colorBuffer =
            (uint32_t *)malloc(sizeof(uint32_t) * WINDOW_WIDTH * WINDOW_HEIGHT);
        if (!colorBuffer)
        {
                fprintf(stderr, "Failed to allocate colorBuffer\n");
                return (1);
		} */

        setup_player();

        while (isRunning)
        {
                SDL_RenderClear(instance.renderer);
                if (poll_events() == 1)
                        break;

                /* Calculate frame time and delta time */
                frameTime = SDL_GetTicks() - ticksLastFrame;
                deltaTime = frameTime / 1000.0f;
                ticksLastFrame = SDL_GetTicks();

                /* Update player position */
                move_player(deltaTime);
                cast_all_rays();

                SDL_SetRenderDrawColor(instance.renderer, 255, 0, 255, 255);
                SDL_RenderClear(instance.renderer);

                generate3DProjection();

                /* Render color buffer */
                renderColorBuffer(instance);
                clearColorBuffer(0xFF000000);

                /* Render game elements */
                render_map(instance);
                render_rays(instance);
                render_player(instance);
                SDL_RenderPresent(instance.renderer);
        }
        /* free(colorBuffer); */

        cleanUp(instance);
        return (0);
}
