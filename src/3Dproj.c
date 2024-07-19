#include "../inc/head.h"

/**
 * generate3DProjection - Generates a 3D projection of the maze
 */
void generate3DProjection(Game *instance)
{
        int k, l, num_tex, distance_from_top;
        float perp_distance, distance_proj_plane, projected_wall_height;
        int wall_strip_height, wall_top_pixel, wall_bottom_pixel;
        int texture_offset_x, texture_offset_y;
        Uint32 texel_color;

        if (!colorBuffer || !textures)
        {
                fprintf(
                    stderr,
                    "Error: colorBuffer or textures are not initialized.\n");
                return;
        }

        for (k = 0; k < NUM_RAYS; k++)
        {
                perp_distance = rays[k].distance *
                                cos(rays[k].ray_angle - p.rotation_angle);
                distance_proj_plane = (WINDOW_WIDTH / 2) / tan(FOV_ANGLE / 2);
                projected_wall_height =
                    (TILE_SIZE / perp_distance) * distance_proj_plane;
                wall_strip_height = (int)projected_wall_height;

                wall_top_pixel = (WINDOW_HEIGHT / 2) - (wall_strip_height / 2);
                wall_top_pixel = wall_top_pixel < 0 ? 0 : wall_top_pixel;
                wall_bottom_pixel =
                    (WINDOW_HEIGHT / 2) + (wall_strip_height / 2);
                wall_bottom_pixel = wall_bottom_pixel > WINDOW_HEIGHT
                                        ? WINDOW_HEIGHT
                                        : wall_bottom_pixel;

                /* Render ceiling */
                for (l = 0; l < wall_top_pixel; l++)
                {
                        if ((WINDOW_WIDTH * l) + k <
                            WINDOW_WIDTH * WINDOW_HEIGHT)
                        {
                                colorBuffer[(WINDOW_WIDTH * l) + k] = 0x9FFCFD;
                        }
                }

                if (rays[k].was_hit_vertical)
                {
                        texture_offset_x =
                            (int)rays[k].wall_hit_y % TEXTURE_HEIGHT;
                }
                else
                {
                        texture_offset_x =
                            (int)rays[k].wall_hit_x % TEXTURE_WIDTH;
                }

                num_tex = rays[k].wall_hit_content - 1;

                // Check for valid texture index
                if (num_tex < 0 || num_tex >= NUM_TEXTURES ||
                    !textures[num_tex])
                {
                        fprintf(stderr,
                                "Error: Invalid texture index or texture not "
                                "loaded.\n");
                        continue;
                }

                // Render wall
                for (l = wall_top_pixel; l < wall_bottom_pixel; l++)
                {
                        distance_from_top =
                            l + (wall_strip_height / 2) - (WINDOW_HEIGHT / 2);
                        texture_offset_y =
                            (int)(distance_from_top *
                                  ((float)TEXTURE_HEIGHT / wall_strip_height));
                        texture_offset_y =
                            texture_offset_y < 0 ? 0 : texture_offset_y;
                        texture_offset_y = texture_offset_y >= TEXTURE_HEIGHT
                                               ? TEXTURE_HEIGHT - 1
                                               : texture_offset_y;
                        texel_color =
                            textures[num_tex]
                                    [(TEXTURE_WIDTH * texture_offset_y) +
                                     texture_offset_x];

                        if ((WINDOW_WIDTH * l) + k <
                            WINDOW_WIDTH * WINDOW_HEIGHT)
                        {
                                colorBuffer[(WINDOW_WIDTH * l) + k] =
                                    texel_color;
                        }
                }

                // Render floor
                for (l = wall_bottom_pixel; l < WINDOW_HEIGHT; l++)
                {
                        if ((WINDOW_WIDTH * l) + k <
                            WINDOW_WIDTH * WINDOW_HEIGHT)
                        {
                                colorBuffer[(WINDOW_WIDTH * l) + k] = 0x75FA61;
                        }
                }
        }

        if (colorBufferTexture)
        {
                SDL_UpdateTexture(colorBufferTexture, NULL, colorBuffer,
                                  xpos * sizeof(Uint32));
                SDL_RenderCopy(instance->renderer, colorBufferTexture, NULL,
                               NULL);
        }
}
