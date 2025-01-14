#ifndef HEAD_H
#define HEAD_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include "player.h"

/* Define constants */
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define MAP_WIDTH 15
#define MAP_HEIGHT 15
#define TILE_SIZE 64
#define MINIMAP_SCALE_FACTOR 0.2
#define TEXTURE_WIDTH 64
#define TEXTURE_HEIGHT 64
#define NUM_TEXTURES 1
#define WOOD_TEXTURE 0xFF0000
#define MINIMAP_SCALE_FACTOR 0.2
#define NUM_RAYS 320
#define FOV_ANGLE (60 * (M_PI / 180))
#define xpos 960
#define ypos 960

/* Structure for the game */
typedef struct Game
{
        SDL_Window *window;
        SDL_Renderer *renderer;
} Game;

/* Structure for rays */
struct Ray
{
        float ray_angle;
        float distance;
        float wall_hit_y;
        float wall_hit_x;
        int wall_hit_content;
        int was_hit_vertical;
        int is_ray_facing_down;
        int is_ray_facing_up;
        int is_ray_facing_left;
        int is_ray_facing_right;
};

/* Global variables */
/* extern int xpos; */
/* extern int ypos; */
extern const int map[MAP_HEIGHT][MAP_WIDTH];
extern Uint32 *colorBuffer;
extern SDL_Texture *colorBufferTexture;
extern Uint32 *textures[NUM_TEXTURES];
extern struct Ray rays[NUM_RAYS];
extern bool isRunning;
extern SDL_Texture *bottom;
extern SDL_Texture *wall;
extern SDL_Texture *win;
extern SDL_Texture *bigfoot;

/* Function prototypes */
int main(int argc, char *argv[]);
int init_instance(Game *instance);
void render_player(Game instance);
void cleanUp(Game instance);
float normalize_angle(float angle);
float distance_between_points(float x1, float y1, float x2, float y2);
void cast_ray(float rayAngle, int stripId);
void cast_all_rays(void);
void render_map(Game instance);
void render_rays(Game instance);
int poll_events(void);
void setup_player(void);
int map_has_wall_at(float x, float y);
void move_player(float deltaTime);
SDL_Texture *LoadTexture(SDL_Renderer *ren, const char *fileName);
void generate3DProjection(Game *instance);
void renderColorBuffer(Game instance);
void clearColorBuffer(Uint32 color);

#endif
