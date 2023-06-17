//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Game Board Generation Functions
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#pragma once
#include "raylib.h"
#include <time.h> // time(NULL)
#include <stdlib.h> // rand()

Image gen_raw_noise(int size, int seed);
Image gen_oceans(int size, int seed, float ocean_threshold);