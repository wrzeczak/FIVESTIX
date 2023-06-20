#pragma once
#include "raylib.h"
#include "pixel_dialog.h"
#include <time.h>

extern Texture2D board_map_texture;
extern Texture2D board_terrain_texture;

void init_render(void);
void render_game(clock_t total_generation_clock_cycles, clock_t total_generation_count, clock_t generation_clock_cycles, clock_t render_clock_cycles, PixelDialog pixel_dialog);