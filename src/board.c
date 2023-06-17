#include "board.h"
#include <stdalign.h>
#include <string.h>

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

    size_t i = 0;
    for(size_t y = 0; y < BOARD_SIZE; y++) {
        for(size_t x = 0; x < BOARD_SIZE; x++, i++) {
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

void set_board_map_pixel_state(size_t index, Color country_color, Color culture_color, Color language_color, BoardPixelId id) {
    board.map_pixel_colors_arrays[MS_COUNTRY][index] = country_color;
    board.map_pixel_colors_arrays[MS_CULTURE][index] = culture_color;
    board.map_pixel_colors_arrays[MS_LANGUAGE][index] = language_color;
    board.ids[index] = id;
}