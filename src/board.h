//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  Board data, utility functions, and types
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#pragma once
#include "consts.h"
#include "raylib.h"
#include <stdlib.h>

typedef enum {
    MS_COUNTRY,
    MS_CULTURE,
    MS_LANGUAGE
} BoardMapState;

typedef struct {
    unsigned short country_id;
    unsigned short culture_id;
    unsigned short language_id;
} BoardPixelId;

// Using SoA here as to cut down on cacheline usage
// All data is indexed in row-major order, i.e x then y
typedef struct {
    // One array of colors for each map state
    _Alignas(64) Color map_pixel_colors_arrays[BOARD_MAP_STATES_COUNT][BOARD_PIXEL_COUNT];

    _Alignas(64) float terrain_heights[BOARD_PIXEL_COUNT];
    _Alignas(64) Color terrain_pixel_colors[BOARD_PIXEL_COUNT];

    // Pixel id information, maybe split into seperate arrays in the future if needed
    _Alignas(64) BoardPixelId ids[BOARD_PIXEL_COUNT];
} Board;

// Global variable used because theres only one board
extern Board board;

void init_board();
void update_board_terrain(int seed, float ocean_threshold);
// This function is mainly for testing/debugging, actual changes to pixel states should be more graceful
void set_board_map_pixel_state(size_t index, Color country_color, Color culture_color, Color language_color, BoardPixelId id);