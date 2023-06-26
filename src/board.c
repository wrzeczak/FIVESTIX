#include "board.h"
#include "raymath.h"
#include "board_gen.h"
#include "pixel_attrib.h"
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

    init_board_gen();
}

static ubyte get_random_byte() {
    return rand() % 256;
}

void update_board_terrain(int seed) {
    srand(seed);

    for (size_t i = 0; i < MAX_COUNTRIES_COUNT; i++) {
        //! TODO: Add proper naming
        country_display_names[i] = "Test";
        country_colors[i] = (Vector4) { get_random_byte() / 255.0f, get_random_byte() / 255.0f, get_random_byte() / 255.0f, 1.0f };
    }

    generate_board();
}