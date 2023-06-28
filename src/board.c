#include "board.h"
#include "raymath.h"
#include "board_gen.h"
#include "pixel_attrib.h"
#include <stdalign.h>
#include <string.h>
#include <stdio.h>

_Alignas(64) Board board;

void init_board(void) {
    init_board_gen();
}

static uint get_random(uint max_exclusive) {
    return rand() % max_exclusive;
}

void update_board_terrain(int seed) {
    srand(seed);

    for (size_t i = 0; i < MAX_COUNTRIES_COUNT; i++) {
        //! TODO: Add proper naming
        country_colors[i] = (Vector4) { get_random(256) / 255.0f, get_random(256) / 255.0f, get_random(256) / 255.0f, 1.0f };
        country_centers[i] = (Vector2) { get_random(BOARD_SIZE) / (float)(BOARD_SIZE - 1), get_random(BOARD_SIZE) / (float)(BOARD_SIZE - 1) };
    }

    generate_board(seed);
}