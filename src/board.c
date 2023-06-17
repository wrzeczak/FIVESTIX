#include "board.h"
#include <stdalign.h>
#include <string.h>

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

void update_terrain_pixel_colors(float ocean_threshold, const float * restrict heightmap) {
    Color * pixel_colors = board.terrain_pixel_colors;

    for(size_t i = 0; i < BOARD_PIXEL_COUNT; i++) {
        //! TODO: If the compiler can't optimize this branch well we may have to do some fuckery
        //! NOTE: The compiler indeed does branch here
        float height = heightmap[i];
        if(height > ocean_threshold) {
            pixel_colors[i] = Fade(GREEN, height);
        } else {
            pixel_colors[i] = Fade(BLUE, height);
        }
    }
}

void set_board_map_pixel_state(size_t index, Color country_color, Color culture_color, Color language_color, BoardPixelId id) {
    board.map_pixel_colors_arrays[MS_COUNTRY][index] = country_color;
    board.map_pixel_colors_arrays[MS_CULTURE][index] = culture_color;
    board.map_pixel_colors_arrays[MS_LANGUAGE][index] = language_color;
    board.ids[index] = id;
}