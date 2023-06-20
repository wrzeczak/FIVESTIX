#include "board.h"
#include "raymath.h"
#define FNL_IMPL
#include "fastnoise.h"
#include <stdalign.h>
#include <string.h>
#include <stdio.h>

_Alignas(64) Board board;

void init_board(void) {
    for(size_t i = 0; i < BOARD_PIXEL_COUNT; i++) {
        // memset(&board.ids, 0, sizeof(board.ids));
        board.ids[i] = (BoardPixelId) {
            NULL_USHORT,
            NULL_USHORT,
            NULL_USHORT
        };
    }

    for(size_t i = 0; i < BOARD_MAP_STATES_COUNT; i++) {
        Color * pixel_colors = board.map_pixel_colors_arrays[i];
        for(size_t j = 0; j < BOARD_PIXEL_COUNT; j++) {
            pixel_colors[j] = WHITE;
        }
    }
}

_Alignas(64) static Vector2 high_points[256];

void update_board_terrain(int seed, float ocean_threshold) {
    size_t high_points_count = 0;

    float * heights = board.terrain_heights;
    Vector2 * gradients = board.terrain_gradients;
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

    float delta = 0.001f;

    {
        size_t i = 0;
        for(float y = 0; y < BOARD_SIZE; y++) {
            for(float x = 0; x < BOARD_SIZE; x++, i++) {
                float height = fnlGetNoise2D(&noise, x, y);
                Vector2 gradient = (Vector2) { (fnlGetNoise2D(&noise, x + delta, y) - height)/delta, (fnlGetNoise2D(&noise, x, y + delta) - height)/delta };

                heights[i] = height;
                gradients[i] = gradient;

                if(height >= 0.0f && high_points_count < 256 && Vector2Length(gradient) <= 0.0005f) {
                    high_points[high_points_count++] = (Vector2) { x, y };
                }

                height += 1;
                height /= 2.0f;
                if(height > ocean_threshold) {
                    pixel_colors[i] = Fade(GREEN, height);
                } else {
                    pixel_colors[i] = Fade(BLUE, height);
                }
            }
        }
    }

    // Take the high points and make rivers
    for(size_t i = 0; i < high_points_count; i++) {
        Vector2 cur_position = high_points[i];

        printf("BEGIN: %f, %f\n", cur_position.x, cur_position.y);

        // cur_position.x += 1.0f;
        // cur_position.y += 1.0f;

        // Using velocity simulation to make river path (suggested by Random we'll see if your idea sucks or not)
        for(size_t j = 0; j < 128 && cur_position.x < BOARD_SIZE && cur_position.y < BOARD_SIZE; j++) {
            //! TODO: Ideas for river algorithm to improve it:
            // Make the rivers larger to allow for nice variation

            size_t index = GET_BOARD_INDEX(cur_position.x, cur_position.y);
            Vector2 velocity = Vector2Scale(Vector2Normalize(gradients[index]), 0.05f);

            cur_position = Vector2Subtract(cur_position, velocity);

            pixel_colors[index] = BLUE;
        }

        printf("END: %f, %f\n", cur_position.x, cur_position.y);
    }
}

void set_board_map_pixel_state(size_t index, Color country_color, Color culture_color, Color language_color, BoardPixelId id) {
    board.map_pixel_colors_arrays[MS_COUNTRY][index] = country_color;
    board.map_pixel_colors_arrays[MS_CULTURE][index] = culture_color;
    board.map_pixel_colors_arrays[MS_LANGUAGE][index] = language_color;
    board.ids[index] = id;
}