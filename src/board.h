//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  Board data, utility functions, and types
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#pragma once
#include "consts.h"
#include "raylib.h"
#include "types.h"
#include <stdlib.h>

typedef enum {
    MS_COUNTRY,
    MS_CULTURE,
    MS_LANGUAGE
} BoardMapState;

typedef struct {
    ushort country_id;
    ushort culture_id;
    ushort language_id;
} BoardPixelId;

// Using SoA here as to cut down on cacheline usage
// All data is indexed in row-major order, i.e x then y
typedef struct {
    // One array of colors for each map state
    _Alignas(64) Color map_pixel_colors_arrays[BOARD_MAP_STATES_COUNT][BOARD_PIXEL_COUNT];

    // Pixel id information, maybe split into seperate arrays in the future if needed
    _Alignas(64) BoardPixelId ids[BOARD_PIXEL_COUNT];
} Board;

// Global variable used because theres only one board
extern Board board;

void init_board(void);
void update_board_terrain(int seed);

#define GET_BOARD_INDEX(X, Y) (((Y) * BOARD_SIZE) + (X))