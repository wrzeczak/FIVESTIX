#include "perlin.h"
#define FNL_IMPL
#include "fastnoise.h"
#include "raymath.h"

//! TODO: Rewrite this to be less bad

static float q(float t) {
    return _fnlInterpQuintic(t);
}

// Derivative of 6t^{5}-15t^{4}+10t^{3}
static float qd(float t) {
    return 30*(t*t*t*t) - 60*(t*t*t) + 30*(t*t);
}

static float get_perlin_grad_x(float x, float y, float k0, float k2, float k4, float k6, float g0, float g1, float g2, float g3) {
    return k0+qd(x)*(g1-g0)+q(x)*(k2-k0)+q(y)*((k4+qd(x)*(g3-g2)+q(x)*(k6-k4))-k0-qd(x)*(g1-g0)-q(x)*(k2-k0));
}

static float get_perlin_grad_y(float x, float y, float k1, float k3, float k5, float k7, float g0, float g1, float g2, float g3) {
    return k1+q(x)*(k3-k1)+(qd(y))*((g2+q(x)*(g3-g2))-g0-q(x)*(g1-g0))+q(y)*(k5+q(x)*(k7-k5)-k1-q(x)*(k3-k1));
}

typedef struct {
    float k0;
    float k1;
    float g;
} GradCoord;

static GradCoord get_grad_coord(int seed, int xPrimed, int yPrimed, float xd, float yd) {
    int hash = _fnlHash2D(seed, xPrimed, yPrimed);
    hash ^= hash >> 15;
    hash &= 127 << 1;

    GradCoord coord;
    coord.k0 = GRADIENTS_2D[hash];
    coord.k1 = GRADIENTS_2D[hash | 1];
    coord.g = xd * coord.k0 + yd * coord.k1;
    return coord;
}

PerlinValue get_single_perlin_2d(int seed, FNLfloat x, FNLfloat y) {
    int x0 = _fnlFastFloor(x);
    int y0 = _fnlFastFloor(y);

    float xd0 = (float)(x - x0);
    float yd0 = (float)(y - y0);
    float xd1 = xd0 - 1;
    float yd1 = yd0 - 1;

    float xs = _fnlInterpQuintic(xd0);
    float ys = _fnlInterpQuintic(yd0);

    x0 *= PRIME_X;
    y0 *= PRIME_Y;
    int x1 = x0 + PRIME_X;
    int y1 = y0 + PRIME_Y;

    GradCoord grad_coord;
    
    grad_coord = get_grad_coord(seed, x0, y0, xd0, yd0); float k0 = grad_coord.k0; float k1 = grad_coord.k1; float g0 = grad_coord.g;
    grad_coord = get_grad_coord(seed, x1, y0, xd1, yd0); float k2 = grad_coord.k0; float k3 = grad_coord.k1; float g1 = grad_coord.g;
    grad_coord = get_grad_coord(seed, x0, y1, xd0, yd1); float k4 = grad_coord.k0; float k5 = grad_coord.k1; float g2 = grad_coord.g;
    grad_coord = get_grad_coord(seed, x1, y1, xd1, yd1); float k6 = grad_coord.k0; float k7 = grad_coord.k1; float g3 = grad_coord.g;

    float xf0 = _fnlLerp(g0, g1, xs);
    float xf1 = _fnlLerp(g2, g3, xs);

    float height = _fnlLerp(xf0, xf1, ys) * 1.4247691104677813f;
    float perlin_grad_x = get_perlin_grad_x(xd0, yd0, k0, k2, k4, k6, g0, g1, g2, g3) * 1.4247691104677813f;
    float perlin_grad_y = get_perlin_grad_y(xd0, yd0, k1, k3, k5, k7, g0, g1, g2, g3) * 1.4247691104677813f;

    // printf("k_0=%f\nk_1=%f\nk_2=%f\nk_3=%f\nk_4=%f\nk_5=%f\nk_6=%f\nk_7=%f\n%f,%f,%f,%f\n", k0, k1, k2, k3, k4, k5, k6, k7, xd0, yd0, height,perlin_grad_x);

    return (PerlinValue) {
        .height = height,
        .gradient = (Vector2) { perlin_grad_x, perlin_grad_y }
    };
}

PerlinValue get_fractal_perlin_noise(fnl_state * state, FNLfloat x, FNLfloat y) {
    _fnlTransformNoiseCoordinate2D(state, &x, &y);

    float x_orig = x;
    float y_orig = y;

    int seed = state->seed;
    PerlinValue sum = {
        .height = 0,
        .gradient = (Vector2) { 0, 0 }
    };
    float amp = _fnlCalculateFractalBounding(state);

    for (int i = 0; i < state->octaves; i++)
    {
        PerlinValue value = get_single_perlin_2d(seed++, x, y);
        float noise = value.height;
        sum.height += noise * amp;

        Vector2 gradient = value.gradient;
        gradient.x *= (x / x_orig);
        gradient.y *= (y / y_orig);

        sum.gradient = Vector2Add(sum.gradient, Vector2Scale(gradient, amp));
        amp *= _fnlLerp(1.0f, _fnlFastMin(noise + 1, 2) * 0.5f, state->weighted_strength);

        x *= state->lacunarity;
        y *= state->lacunarity;
        amp *= state->gain;
    }

    return sum;
}