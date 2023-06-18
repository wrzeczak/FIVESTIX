#include "raylib.h"

#include "board.h"
#include "consts.h"
#include "camera.h"
#include "ext.h"
#include "fastnoise.h"
#include "raymath.h"

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdalign.h>

static size_t get_index(Vector2 pos) {
    return (pos.y * BOARD_SIZE) + pos.x;
}

#define WINDOWED_SCREEN_WIDTH 1280
#define WINDOWED_SCREEN_HEIGHT 720

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

typedef struct {
    float height;
    Vector2 gradient;
} PerlinValue;

static PerlinValue get_single_perlin_2d(int seed, FNLfloat x, FNLfloat y) {
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

static PerlinValue get_fractal_perlin_noise(fnl_state *state, FNLfloat x, FNLfloat y) {
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

int main(void) {
    // Using seperate bool for fullscreen since if you toggle the fullscreen without setting window size first it messes with your monitor resolution
    bool fullscreen = false;

    srand(time(NULL));

    InitWindow(WINDOWED_SCREEN_WIDTH, WINDOWED_SCREEN_HEIGHT, "Hello!");

    // Enable vsync
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

    Texture2D map_texture = init_texture_with_size(BOARD_SIZE, BOARD_SIZE);
    Texture2D terrain_texture = init_texture_with_size(BOARD_SIZE, BOARD_SIZE);

    SetTextureFilter(map_texture, TEXTURE_FILTER_POINT);
    SetTextureFilter(terrain_texture, TEXTURE_FILTER_POINT);

    size_t map_state = MS_COUNTRY;

    // Don't do this until we actually want random terrain
    // int generation_seed = rand();
    int generation_seed = 0;

    clock_t generation_start_clock = clock();
    update_board_terrain(generation_seed, 0.33f);
    clock_t generation_end_clock = clock();

    clock_t generation_clock_cycles = generation_end_clock - generation_start_clock;

    UpdateTexture(terrain_texture, board.terrain_pixel_colors);

    init_camera();
    init_board();

    // At the moment the order of initialization before here for everything is irrelevant

    set_board_map_pixel_state(get_index((Vector2) { 0, 0 }), RED, Fade(RED, 0.5f), Fade(RED, 0.25f), (BoardPixelId) {
        1, 1, 1
    });
    set_board_map_pixel_state(get_index((Vector2) { 10, 10 }), ORANGE, Fade(ORANGE, 0.5f), Fade(ORANGE, 0.25f), (BoardPixelId) {
        1, 1, 1
    });
    set_board_map_pixel_state(get_index((Vector2) { 10, 5 }), YELLOW, Fade(YELLOW, 0.5f), Fade(YELLOW, 0.25f), (BoardPixelId) {
        1, 1, 1
    });

    clock_t render_clock_cycles = 0;

    // Testing perlin noise stuff
    Texture2D perlin_texture = init_texture_with_size(1024, 1024);
    Texture2D perlin_grad_texture = init_texture_with_size(1024, 1024);
    SetTextureFilter(perlin_texture, TEXTURE_FILTER_POINT);
    SetTextureFilter(perlin_grad_texture, TEXTURE_FILTER_POINT);

    {
        Color * pixel_colors = malloc(1024*1024*sizeof(Color));
        Color * pixel_colors_1 = malloc(1024*1024*sizeof(Color));

        fnl_state noise = fnlCreateState();
        noise.noise_type = FNL_NOISE_PERLIN;
        noise.seed = generation_seed;

        // cue the magic numbers - discovered while messing around with the fnl GUI
        noise.fractal_type = FNL_FRACTAL_FBM;
        noise.frequency = 0.005f;
        noise.octaves = 6;
        noise.gain = 0.40f;
        noise.weighted_strength = -0.70f;
        {
            size_t i = 0;
            for(float y = 0; y < 1024; y++) {
                for(float x = 0; x < 1024; x++, i++) {
                    PerlinValue val = get_fractal_perlin_noise(&noise, x/16.0f, y/16.0f);

                    float height = val.height;
                    height += 1;
                    height /= 2.0f;

                    float grad = Vector2Length(val.gradient);

                    pixel_colors[i] = (Color) { height * 255.0f, height * 255.0f, height * 255.0f, 255 };
                    pixel_colors_1[i] = (Color) { grad * 255.0f, grad * 255.0f, grad * 255.0f, 255 };
                }
            }
        }

        UpdateTexture(perlin_texture, pixel_colors);
        UpdateTexture(perlin_grad_texture, pixel_colors_1);
    }

    //
    
    while(!WindowShouldClose()) {

        // Ugly hack, unless you know how to make fullscreen work correctly do not touch this
        if(IsKeyPressed(KEY_F11)) {
            fullscreen = !fullscreen;
            int width;
            int height;
            if(fullscreen) {
                int monitor = GetCurrentMonitor();
                width = GetMonitorWidth(monitor);
                height = GetMonitorHeight(monitor);
            } else {
                width = WINDOWED_SCREEN_WIDTH;
                height = WINDOWED_SCREEN_HEIGHT;
            }
            // Doing this twice because it doesnt work if only done once
            SetWindowSize(width, height);
            ToggleFullscreen();
            SetWindowSize(width, height);
        }

        if(IsKeyPressed(KEY_SPACE)) {
            map_state++;
            map_state %= 3;

            generation_seed++;
            generation_start_clock = clock();
            update_board_terrain(generation_seed, 0.33f);
            generation_end_clock = clock();
            generation_clock_cycles = generation_end_clock - generation_start_clock;

            UpdateTexture(terrain_texture, board.terrain_pixel_colors);
        }

        update_camera();

        // Doesn't work on Windows because clock() is broken, perhaps we just scrap this whole frame-time thing?
        clock_t render_start_clock = clock();
        //! WARN: May need to enter texture mode if we add more textures in the future
        UpdateTexture(map_texture, board.map_pixel_colors_arrays[map_state]);

        BeginDrawing();

            BeginMode2D(camera);
                ClearBackground(BLACK);

                // DrawTexturePro(map_texture, (Rectangle) { 0.0f, 0.0f, map_texture.width, map_texture.height }, (Rectangle) { 0.0f, 0.0f, BOARD_RECT_SIZE, BOARD_RECT_SIZE }, (Vector2) { 0.0f, 0.0f }, 0.0f, WHITE);
                // DrawTextureEx(terrain_texture, (Vector2) { BOARD_SIZE * -1.5f, 0 }, 0.0f, 1.5f, WHITE);
                DrawTextureEx(perlin_texture, (Vector2) { 0, 0 }, 0.0f, 1.0f, WHITE);
                DrawTextureEx(perlin_grad_texture, (Vector2) { -1024.0f, 0 }, 0.0f, 1.0f, WHITE);

                // Draw perlin noise tests

                
            EndMode2D();
            
            DrawText(TextFormat("GEN: %ld", generation_clock_cycles), 10, GetRenderHeight() - 70, 20, GREEN);
            DrawText(TextFormat("RND: %ld", render_clock_cycles), 10, GetRenderHeight() - 50, 20, GREEN);

            DrawFPS(10, GetRenderHeight() - 25);

        EndDrawing();

        clock_t render_end_clock = clock();
        render_clock_cycles = render_end_clock - render_start_clock;
    }
}