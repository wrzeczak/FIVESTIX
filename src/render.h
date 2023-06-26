#pragma once
#include "raylib.h"
#include "pixel_dialog.h"
#include <stddef.h>

extern Texture2D board_terrain_texture;
extern Texture2D board_country_map_texture;

void init_render(void);
void render_game(double total_generation_duration, size_t total_generation_count, double generation_duration, double render_duration, PixelDialog pixel_dialog);