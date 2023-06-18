#include "board.h"
#include "perlin.h"
#include "raymath.h"
#include <stdalign.h>
#include <string.h>
#include <stdio.h>

_Alignas(64) Board board;

void init_board() {
    memset(&board.ids, 0, sizeof(board.ids));

    for(size_t i = 0; i < BOARD_MAP_STATES_COUNT; i++) {
        Color * pixel_colors = board.map_pixel_colors_arrays[i];
        for(size_t j = 0; j < BOARD_PIXEL_COUNT; j++) {
            pixel_colors[j] = WHITE;
        }
    }
}

void update_board_terrain(int seed, float ocean_threshold) {
    size_t high_points_count = 0;
    struct {
        size_t x;
        size_t y;
    } high_points[256];

    float * heights = board.terrain_heights;
    Color * pixel_colors = board.terrain_pixel_colors;

    fnl_state noise = fnlCreateState();
    noise.noise_type = FNL_NOISE_PERLIN;
    noise.seed = seed;

    // cue the magic numbers - discovered while messing around with the fnl GUI
    noise.fractal_type = FNL_FRACTAL_FBM;
    noise.frequency = 0.005f;
    noise.octaves = 6;
    noise.gain = 0.40f;
    noise.weighted_strength = -0.70f;

    {
        size_t i = 0;
        for(float y = 0; y < BOARD_SIZE; y++) {
            for(float x = 0; x < BOARD_SIZE; x++, i++) {
                // Not sure if my code just doesn't work or if this algorithm doesn't really do what we need it to do but idk if we should keep using gradients to find high points
                PerlinValue value = get_fractal_perlin_noise(&noise, x, y);
                float height = value.height;
                height += 1;
                height /= 2.0f;
            
                if(height >= 0.5f && high_points_count < 256 && Vector2Length(value.gradient) <= 0.1f) {
                    high_points[high_points_count].x = x;
                    high_points[high_points_count++].y = y;
                }

                heights[i] = height;

                // pixel_colors[i] = (Color) { height * 255.0f, height * 255.0f, height * 255.0f, 255 };

                if(height > ocean_threshold) {
                    pixel_colors[i] = Fade(GREEN, height);
                } else {
                    pixel_colors[i] = Fade(BLUE, height);
                }
            }
        }
    }

    // Set seed for bad-ish river algorithm
    srand(seed);

    // Take the high points and make rivers
    for(size_t i = 0; i < high_points_count; i++) {
        size_t x = high_points[i].x;
        size_t y = high_points[i].y;

        for(size_t j = 0; j < 128; j++) {
            //! TODO: This is bad
            //! TODO: Ideas for river algorithm to improve it:
            // Add a bit of randomness to which direction it picks (i.e not always the lowest)
            // As mentioned above, it may be lead to better results to scrap the gradient thing and just randomly find points above a certain height, certainly would be a lot faster

            size_t index = (y * BOARD_SIZE) + x;

            // Find the nearest lowest point
            float comp_height0 = x + 1 < BOARD_SIZE ? heights[index + 1] : 2.0f;
            float comp_height1 = x - 1 < BOARD_SIZE ? heights[index - 1] : 2.0f;
            float comp_height2 = y + 1 < BOARD_SIZE ? heights[index + BOARD_SIZE] : 2.0f;
            float comp_height3 = y - 1 < BOARD_SIZE ? heights[index - BOARD_SIZE] : 2.0f;

            if(comp_height0 <= comp_height1 && comp_height0 <= comp_height2 && comp_height0 <= comp_height3 && comp_height0 != 2.0f) {
                x++;
                pixel_colors[index] = BLUE;
                continue;
            }

            if(comp_height1 <= comp_height2 && comp_height1 <= comp_height3 && comp_height1 != 2.0f) {
                x--;
                pixel_colors[index] = BLUE;
                continue;
            }

            if(comp_height2 <= comp_height3 && comp_height2 != 2.0f) {
                y++;
                pixel_colors[index] = BLUE;
                continue;
            }

            if(comp_height3 != 2.0f) {
                y--;
                pixel_colors[index] = BLUE;
                continue;
            }

            // We have only higher points around us, stop
            break;
        }
    }
}

void set_board_map_pixel_state(size_t index, Color country_color, Color culture_color, Color language_color, BoardPixelId id) {
    board.map_pixel_colors_arrays[MS_COUNTRY][index] = country_color;
    board.map_pixel_colors_arrays[MS_CULTURE][index] = culture_color;
    board.map_pixel_colors_arrays[MS_LANGUAGE][index] = language_color;
    board.ids[index] = id;
}