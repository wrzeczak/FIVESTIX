#include "raylib.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

// Either X_PIXELS_COUNT or Y_PIXELS_COUNT should be kept a multiple of 64 so as to keep the color arrays aligned to 64 bytes

#define X_PIXELS_COUNT 64
#define Y_PIXELS_COUNT 64
#define PIXELS_COUNT (X_PIXELS_COUNT * Y_PIXELS_COUNT)

#define PIXEL_WIDTH 15
#define PIXEL_HEIGHT 15

#define BOARD_WIDTH (X_PIXELS_COUNT*PIXEL_WIDTH)
#define BOARD_HEIGHT (Y_PIXELS_COUNT*PIXEL_HEIGHT)

#define X_OFFSET (SCREEN_WIDTH - BOARD_WIDTH) / 2
#define Y_OFFSET (SCREEN_HEIGHT - BOARD_HEIGHT) / 2

#define MAP_STATES_COUNT 3