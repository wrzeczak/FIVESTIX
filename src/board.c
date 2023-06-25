#include "board.h"
#include "raymath.h"
#include "board_gen.h"
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

    init_board_gen();
}

void update_board_terrain(int seed) {
    generate_board();
}

void set_board_map_pixel_state(size_t index, Color country_color, Color culture_color, Color language_color, BoardPixelId id) {
    board.map_pixel_colors_arrays[MS_COUNTRY][index] = country_color;
    board.map_pixel_colors_arrays[MS_CULTURE][index] = culture_color;
    board.map_pixel_colors_arrays[MS_LANGUAGE][index] = language_color;
    board.ids[index] = id;
}