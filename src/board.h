//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Game Board Generation Functions
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#pragma once
#include "raylib.h"
#include <stddef.h>

void gen_heightmap(int seed, size_t size, float * heightmap);
void gen_terrain_image(float ocean_threshold, size_t size, const float * restrict heightmap, Color * pixel_colors);