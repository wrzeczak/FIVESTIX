#pragma once
#include "fastnoise.h"
#include "raymath.h"

typedef struct {
    float height;
    Vector2 gradient;
} PerlinValue;

static PerlinValue get_fractal_perlin_noise(fnl_state * state, FNLfloat x, FNLfloat y);