#pragma once

// BOARD_SIZE should be kept a multiple of 64 so as to keep the color arrays aligned to 64 bytes

#define BOARD_SIZE 256
#define BOARD_PIXEL_COUNT (BOARD_SIZE * BOARD_SIZE)

// Board graphical constants

#define BOARD_PIXEL_SIZE 15
#define BOARD_RECT_SIZE (BOARD_SIZE*BOARD_PIXEL_SIZE)

//

#define MAP_STATES_COUNT 3