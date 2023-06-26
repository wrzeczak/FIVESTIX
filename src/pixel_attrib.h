#pragma once
#include "raylib.h"
#include "raymath.h"

extern Vector4 country_colors[];

// Using string pointers at the moment, may move to fixed size buffer depending on how we deal with display names
extern const char * country_display_names[];
extern const char * culture_display_names[];
extern const char * language_display_names[];