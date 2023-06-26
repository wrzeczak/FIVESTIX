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
    uint country_id;
    uint culture_id;
    uint language_id;
} BoardPixelId;

// Using SoA here as to cut down on cacheline usage
// All data is indexed in row-major order, i.e x then y
typedef struct {
    // Pixel id information, maybe split into seperate arrays in the future if needed
    _Alignas(64) BoardPixelId ids[BOARD_PIXEL_COUNT];
} Board;

// Global variable used because theres only one board
extern Board board;

void init_board(void);
void update_board_terrain(int seed);

#define GET_BOARD_INDEX(X, Y) (((Y) * BOARD_SIZE) + (X))