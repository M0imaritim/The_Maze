#include "../inc/head.h"

/**
 * clearColorBuffer - clears the color buffer
 * @color: the color
 * Return: void
 */
void clearColorBuffer(Uint32 color)
{
        int x, y;

        // Check if colorBuffer is initialized
        if (!colorBuffer)
        {
                fprintf(stderr, "Error: colorBuffer is not initialized.\n");
                return;
        }

        // Ensure xpos and ypos are positive and within bounds
        if (xpos <= 0 || ypos <= 0)
        {
                fprintf(stderr, "Error: Invalid dimensions for colorBuffer.\n");
                return;
        }

        for (x = 0; x < xpos; x++)
        {
                for (y = 0; y < ypos; y++)
                {
                        if ((xpos * y) + x < xpos * ypos)
                        {
                                colorBuffer[(xpos * y) + x] = color;
                        }
                        else
                        {
                                fprintf(stderr,
                                        "Error: Out-of-bounds access in "
                                        "colorBuffer.\n");
                                return;
                        }
                }
        }
}

/**
 * renderColorBuffer - renders the color buffer
 * @instance: Game instance
 * Return: void
 */
void renderColorBuffer(Game instance)
{
        // Check if colorBufferTexture and colorBuffer are initialized
        if (!colorBufferTexture || !colorBuffer)
        {
                fprintf(stderr,
                        "Error: colorBufferTexture or colorBuffer is not "
                        "initialized.\n");
                return;
        }

        // Ensure xpos is positive and within bounds
        if (xpos <= 0)
        {
                fprintf(stderr, "Error: Invalid texture width (xpos).\n");
                return;
        }

        if (SDL_UpdateTexture(colorBufferTexture, NULL, colorBuffer,
                              xpos * sizeof(Uint32)) != 0)
        {
                fprintf(stderr, "Error: SDL_UpdateTexture failed: %s\n",
                        SDL_GetError());
                return;
        }

        if (SDL_RenderCopy(instance.renderer, colorBufferTexture, NULL, NULL) !=
            0)
        {
                fprintf(stderr, "Error: SDL_RenderCopy failed: %s\n",
                        SDL_GetError());
        }
}
