#include "board.h"
#include "raymath.h"
#include <stdalign.h>
#include <string.h>
#include <stdio.h>

#include "fastnoise.h"
#define FML_IMPL

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
                float height = fnlGetNoise2D(&noise, x, y);
                height += 1;
                height /= 2.0f;

                heights[i] = height;

                if(height > ocean_threshold) {
                    pixel_colors[i] = Fade(GREEN, height);
                } else {
                    pixel_colors[i] = Fade(BLUE, height);
                }
            }
        }
    }

    // Gradient ascent to find high points
    // Upper bound in case algorithm doesnt stop
    Vector2 cur_pos = { 50.0f, 50.0f };

    size_t index = 0;
    for(size_t iter = 0; iter < 1024; iter++) {
        index = (cur_pos.y * BOARD_SIZE) + cur_pos.x;

        // Don't want to go out of bounds, this also follows from all local maxima and minima not being on the edge of the heightmap
        // Note that for unsigned integers, it can be the case that k >= n but not k + s >= n
        if (index >= BOARD_PIXEL_COUNT || index + BOARD_SIZE >= BOARD_PIXEL_COUNT) {
            index = -1;
            break;
        }

        float height = heights[index];

        Vector2 height_delta = { heights[index + 1] - height, heights[index + BOARD_SIZE] - height };

        // Not really standard for gradient ascent afaik, but this makes it takes less steps
        height_delta = Vector2Scale(height_delta, 32.0f);

        cur_pos = Vector2Subtract(cur_pos, height_delta);

        // printf("%f, %f\n", cur_pos.x, cur_pos.y);
    }

    if (index == -1) {
        return;
    }

    // index = (cur_pos.y * BOARD_SIZE) + cur_pos.x;
    
    {
        for(size_t y = cur_pos.y; y <= cur_pos.y + 5 && y < BOARD_SIZE; y++) {
            for (size_t x = cur_pos.x; x <= cur_pos.x + 5 && x < BOARD_SIZE; x++) {
                size_t i = (y * BOARD_SIZE) + x;
                pixel_colors[i] = RED;
            }
        }
    }
}

void set_board_map_pixel_state(size_t index, Color country_color, Color culture_color, Color language_color, BoardPixelId id) {
    board.map_pixel_colors_arrays[MS_COUNTRY][index] = country_color;
    board.map_pixel_colors_arrays[MS_CULTURE][index] = culture_color;
    board.map_pixel_colors_arrays[MS_LANGUAGE][index] = language_color;
    board.ids[index] = id;
}