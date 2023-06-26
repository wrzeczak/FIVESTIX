//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Raylib extension functions, these functions use rlgl to provide more raylib functionality
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#pragma once
#include "types.h"
#include "raylib.h"

Texture2D init_texture_with_size(int width, int height);
void read_texture_pixels(uint texture_id, int width, int height, int format, void* buffer);