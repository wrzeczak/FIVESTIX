#pragma once

// BOARD_SIZE should be kept a multiple of 64 so as to keep the color arrays aligned to 64 bytes

#define BOARD_SIZE 256
#define BOARD_PIXEL_COUNT (BOARD_SIZE * BOARD_SIZE)

// Board graphical constants

#define BOARD_PIXEL_SIZE 15
#define BOARD_RECT_SIZE (BOARD_SIZE*BOARD_PIXEL_SIZE)

//

#define BOARD_MAP_STATES_COUNT 3

#define NULL_UCHAR ((uchar)-1)
#define NULL_USHORT ((ushort)-1)
#define NULL_UINT ((uint)-1)
#define NULL_ULONG ((ulong)-1)

#define WINDOWED_SCREEN_WIDTH 1280
#define WINDOWED_SCREEN_HEIGHT 720

#define MAX_COUNTRIES_COUNT 8