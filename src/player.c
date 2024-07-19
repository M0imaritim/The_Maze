#include "../inc/head.h"

Uint32 *wallTexture = NULL;
Uint32 *textures[NUM_TEXTURES];

/**
 * setup_player - sets characteristics of player
 * @instance: Game instance
 * Return: void
 */
void setup_player(void)
{
        // Set player characteristics
        p.x = (xpos * 6.2) / 7;
        p.y = (ypos * 6.2) / 7;
        p.width = 10;
        p.height = 10;
        p.turnDirection = 1;
        p.walkDirection = 0;
        p.rotation_angle = M_PI / 2;
        p.moveSpeed = 100;
        p.turnSpeed = 25 * (M_PI / 180);

        // Initialize textures array
        for (int i = 0; i < NUM_TEXTURES; i++)
        {
                textures[i] = NULL;
        }

        // Load texture (ensure WALL_TEXTURE is a valid texture pointer or path)
        textures[0] =
            (Uint32 *)WOOD_TEXTURE;  // Assuming WOOD_TEXTURE is a valid pointer
}

/**
 * map_has_wall_at - detects wall collision
 * @x: position x
 * @y: position y
 * Return: 0 if true, 1 if false
 */
int map_has_wall_at(float x, float y)
{
        int mapGridIndexX, mapGridIndexY;

        if (x < 0 || x >= xpos || y < 0 || y >= ypos)  // Fixed bounds check
                return (0);

        mapGridIndexX = floor(x / TILE_SIZE);
        mapGridIndexY = floor(y / TILE_SIZE);

        if (mapGridIndexX < 0 || mapGridIndexX >= MAP_WIDTH ||
            mapGridIndexY < 0 || mapGridIndexY >= MAP_HEIGHT)
        {
                fprintf(stderr, "Error: Map index out of bounds.\n");
                return (0);
        }

        return (map[mapGridIndexY][mapGridIndexX] != 0);
}

/**
 * move_player - moving player
 * @deltaTime: time
 * Return: nothing
 */
void move_player(float deltaTime)
{
        float moveStep, newPlayerX, newPlayerY;

        p.rotation_angle += p.turnDirection * p.turnSpeed * 1.10 * deltaTime;
        moveStep = p.walkDirection * p.moveSpeed * deltaTime;

        newPlayerX = p.x + cos(p.rotation_angle) * moveStep;
        newPlayerY = p.y + sin(p.rotation_angle) * moveStep;

        if (!map_has_wall_at(newPlayerX, newPlayerY))
        {
                p.x = newPlayerX;
                p.y = newPlayerY;
        }
}

/**
 * LoadTexture - loads a texture from a file
 * @ren: SDL renderer
 * @fileName: path to the file
 * Return: SDL texture
 */
SDL_Texture *LoadTexture(SDL_Renderer *ren, const char *fileName)
{
        if (!ren || !fileName)
        {
                fprintf(stderr, "Error: Invalid renderer or file name.\n");
                return NULL;
        }

        SDL_Surface *tmpSurface = IMG_Load(fileName);
        if (!tmpSurface)
        {
                fprintf(stderr, "Error: Unable to load image %s: %s\n",
                        fileName, IMG_GetError());
                return NULL;
        }

        SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, tmpSurface);
        if (!tex)
        {
                fprintf(stderr, "Error: Unable to create texture: %s\n",
                        SDL_GetError());
        }

        SDL_FreeSurface(tmpSurface);

        return tex;
}
