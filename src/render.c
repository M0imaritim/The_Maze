#include <limits.h>

#include "../inc/head.h"

struct Player p;
struct Ray rays[NUM_RAYS];

/**
 * normalize_angle - normalizes angle
 * @angle: mad angle
 * Return: normal angle
 */
float normalize_angle(float angle)
{
        angle = remainder(angle, (M_PI * 2));
        if (angle < 0)
                angle = (M_PI * 2) + angle;
        return (angle);
}

/**
 * distance_between_points - measures distance between points
 * @x1: first x point
 * @y1: first y point
 * @x2: second x point
 * @y2: second y point
 * Return: the distance
 */
float distance_between_points(float x1, float y1, float x2, float y2)
{
        return (sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));
}

/**
 * cast_ray - casts rays
 * @ray_angle: the ray angle
 * @strip_id: counter
 */
void cast_ray(float ray_angle, int strip_id)
{
        int is_ray_facing_down, is_ray_facing_up, is_ray_facing_right;
        int is_ray_facing_left, found_horz_wall_hit, horz_wall_content;
        float xintercept, yintercept, xstep, ystep;
        float horz_wall_hit_x, horz_wall_hit_y, next_horz_touch_x;
        float next_horz_touch_y, horz_hit_distance, vert_hit_distance;
        int found_vert_wall_hit, vert_wall_content;
        float vert_wall_hit_x, vert_wall_hit_y, next_vert_touch_x;
        float next_vert_touch_y;

        ray_angle = normalize_angle(ray_angle);
        is_ray_facing_down = ray_angle > 0 && ray_angle < M_PI;
        is_ray_facing_up = !is_ray_facing_down;
        is_ray_facing_right = ray_angle < 0.5 * M_PI || ray_angle > 1.5 * M_PI;
        is_ray_facing_left = !is_ray_facing_right;

        /* Horizontal ray-grid intersection code */
        found_horz_wall_hit = 0;
        horz_wall_hit_x = 0;
        horz_wall_hit_y = 0;
        horz_wall_content = 0;

        /* Finding the y-coordinate of the closest horizontal grid intersection
         */
        yintercept = floor(p.y / TILE_SIZE) * TILE_SIZE;
        yintercept += is_ray_facing_down ? TILE_SIZE : 0;

        /* Finding the x-coordinate of the closest horizontal grid intersection
         */
        xintercept = p.x + (yintercept - p.y) / tan(ray_angle);

        /* Calculate the increment xstep and ystep */
        ystep = TILE_SIZE;
        ystep *= is_ray_facing_up ? -1 : 1;

        xstep = TILE_SIZE / tan(ray_angle);
        xstep *= (is_ray_facing_left && xstep > 0) ? -1 : 1;
        xstep *= (is_ray_facing_right && xstep < 0) ? -1 : 1;

        next_horz_touch_x = xintercept;
        next_horz_touch_y = yintercept;

        /* Incrementing xstep and ystep until finding a wall */
        while (next_horz_touch_x >= 0 && next_horz_touch_x <= xpos &&
               next_horz_touch_y >= 0 && next_horz_touch_y <= ypos)
        {
                float x_to_check = next_horz_touch_x;
                float y_to_check =
                    next_horz_touch_y + (is_ray_facing_up ? -1 : 0);

                if (map_has_wall_at(x_to_check, y_to_check))
                {
                        /* found a wall hit */
                        horz_wall_hit_x = next_horz_touch_x;
                        horz_wall_hit_y = next_horz_touch_y;
                        horz_wall_content =
                            map[(int)floor(y_to_check / TILE_SIZE)]
                               [(int)floor(x_to_check / TILE_SIZE)];
                        found_horz_wall_hit = 1;
                        break;
                }
                else
                {
                        next_horz_touch_x += xstep;
                        next_horz_touch_y += ystep;
                }
        }
        found_vert_wall_hit = 0;
        vert_wall_hit_x = 0;
        vert_wall_hit_y = 0;
        vert_wall_content = 0;

        xintercept = floor(p.x / TILE_SIZE) * TILE_SIZE;
        xintercept += is_ray_facing_right ? TILE_SIZE : 0;
        yintercept = p.y + (xintercept - p.x) * tan(ray_angle);
        xstep = TILE_SIZE;
        xstep *= is_ray_facing_left ? -1 : 1;

        ystep = TILE_SIZE * tan(ray_angle);
        ystep *= (is_ray_facing_up && ystep > 0) ? -1 : 1;
        ystep *= (is_ray_facing_down && ystep < 0) ? -1 : 1;

        next_vert_touch_x = xintercept;
        next_vert_touch_y = yintercept;

        while (next_vert_touch_x >= 0 && next_vert_touch_x <= xpos &&
               next_vert_touch_y >= 0 && next_vert_touch_y <= ypos)
        {
                float x_to_check =
                    next_vert_touch_x + (is_ray_facing_left ? -1 : 0);
                float y_to_check = next_vert_touch_y;

                if (map_has_wall_at(x_to_check, y_to_check))
                {
                        vert_wall_hit_x = next_vert_touch_x;
                        vert_wall_hit_y = next_vert_touch_y;
                        vert_wall_content =
                            map[(int)floor(y_to_check / TILE_SIZE)]
                               [(int)floor(x_to_check / TILE_SIZE)];
                        found_vert_wall_hit = 1;
                        break;
                }
                else
                {
                        next_vert_touch_x += xstep;
                        next_vert_touch_y += ystep;
                }
        }

        horz_hit_distance = found_horz_wall_hit ? distance_between_points(
                                                      p.x, p.y, horz_wall_hit_x,
                                                      horz_wall_hit_y)
                                                : INT_MAX;
        vert_hit_distance = found_vert_wall_hit ? distance_between_points(
                                                      p.x, p.y, vert_wall_hit_x,
                                                      vert_wall_hit_y)
                                                : INT_MAX;
        if (vert_hit_distance < horz_hit_distance)
        {
                rays[strip_id].distance = vert_hit_distance;
                rays[strip_id].wall_hit_x = vert_wall_hit_x;
                rays[strip_id].wall_hit_y = vert_wall_hit_y;
                rays[strip_id].wall_hit_content = vert_wall_content;
                rays[strip_id].was_hit_vertical = 1;
        }
        else
        {
                rays[strip_id].distance = horz_hit_distance;
                rays[strip_id].wall_hit_x = horz_wall_hit_x;
                rays[strip_id].wall_hit_y = horz_wall_hit_y;
                rays[strip_id].wall_hit_content = horz_wall_content;
                rays[strip_id].was_hit_vertical = 0;
        }
        rays[strip_id].ray_angle = ray_angle;
        rays[strip_id].is_ray_facing_down = is_ray_facing_down;
        rays[strip_id].is_ray_facing_up = is_ray_facing_up;
        rays[strip_id].is_ray_facing_left = is_ray_facing_left;
        rays[strip_id].is_ray_facing_right = is_ray_facing_right;
}

/**
 * cast_all_rays - casts all the rays
 * Return: void
 */
void cast_all_rays(void)
{
        int strip_id;
        float ray_angle;

        ray_angle = p.rotation_angle - (FOV_ANGLE / 2);

        for (strip_id = 0; strip_id < NUM_RAYS; strip_id++)
        {
                cast_ray(ray_angle, strip_id);
                ray_angle += FOV_ANGLE / NUM_RAYS;
        }
}

SDL_Texture *bottom = NULL;
SDL_Texture *wall = NULL;
SDL_Texture *win = NULL;
SDL_Texture *bigfoot = NULL;

/**
 * render_map - rendering the map
 * @instance: Game instance
 * Return: void
 */
void render_map(Game instance)
{
        // Load textures
        bottom = LoadTexture(instance.renderer, "assets/wood.png");
        wall = LoadTexture(instance.renderer, "assets/colorstone.png");
        win = LoadTexture(instance.renderer, "assets/mossy.png");
        bigfoot = LoadTexture(instance.renderer, "assets/eagle.png");

        if (!bottom || !wall || !win || !bigfoot)
        {
                fprintf(stderr,
                        "Error: Unable to load one or more textures.\n");
                return;
        }

        int i, j, tile_x, tile_y, tile_color;
        int type = 0;

        for (i = 0; i < MAP_HEIGHT; i++)
        {
                for (j = 0; j < MAP_HEIGHT; j++)
                {
                        tile_x = j * TILE_SIZE;
                        tile_y = i * TILE_SIZE;
                        tile_color = map[i][j] != 0 ? 0xFFFFFFFF : 0x00000000;

                        SDL_SetRenderDrawColor(instance.renderer, tile_color,
                                               tile_color, tile_color, 255);

                        SDL_Rect map_tile_rect = {
                            tile_x * MINIMAP_SCALE_FACTOR,
                            tile_y * MINIMAP_SCALE_FACTOR,
                            TILE_SIZE * MINIMAP_SCALE_FACTOR,
                            TILE_SIZE * MINIMAP_SCALE_FACTOR,
                        };

                        switch (type)
                        {
                                case 0:
                                        SDL_RenderCopy(instance.renderer,
                                                       bottom, NULL,
                                                       &map_tile_rect);
                                        break;
                                case 1:
                                        SDL_RenderCopy(instance.renderer, wall,
                                                       NULL, &map_tile_rect);
                                        break;
                                case 2:
                                        SDL_RenderCopy(instance.renderer, win,
                                                       NULL, &map_tile_rect);
                                        break;
                                case 3:
                                        SDL_RenderCopy(instance.renderer,
                                                       bigfoot, NULL,
                                                       &map_tile_rect);
                                        break;
                                default:
                                        break;
                        }
                        SDL_RenderFillRect(instance.renderer, &map_tile_rect);
                }
        }

        // Clean up textures
        SDL_DestroyTexture(bottom);
        SDL_DestroyTexture(wall);
        SDL_DestroyTexture(win);
        SDL_DestroyTexture(bigfoot);
        bottom = NULL;
        wall = NULL;
        win = NULL;
        bigfoot = NULL;
}

/**
 * render_rays - renders rays
 * @instance: Game instance
 * Return: void
 */
void render_rays(Game instance)
{
        int i;

        SDL_SetRenderDrawColor(instance.renderer, 255, 0, 0, 255);
        for (i = 0; i < NUM_RAYS; i++)
        {
                SDL_RenderDrawLine(instance.renderer,
                                   MINIMAP_SCALE_FACTOR * p.x,
                                   MINIMAP_SCALE_FACTOR * p.y,
                                   MINIMAP_SCALE_FACTOR * rays[i].wall_hit_x,
                                   MINIMAP_SCALE_FACTOR * rays[i].wall_hit_y);
        }
}
