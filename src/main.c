#include "raylib.h"

#include "ecs.h"
#include "board.h"
#include "consts.h"
#include "camera.h"
#include "ext.h"

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdalign.h>

static size_t get_index(Vector2 pos) {
    return (pos.y * BOARD_SIZE) + pos.x;
}

static void init_map() {
    memset(&map.ids, 0, sizeof(map.ids));

    for(size_t i = 0; i < MAP_STATES_COUNT; i++) {
        Color * pixel_colors = map.colors[i];
        for(size_t j = 0; j < BOARD_PIXEL_COUNT; j++) {
            pixel_colors[j] = WHITE;
        }
    }
}

#define WINDOWED_SCREEN_WIDTH 1280
#define WINDOWED_SCREEN_HEIGHT 720

_Alignas(64) static float heightmap[BOARD_PIXEL_COUNT];
_Alignas(64) static Color terrain_pixel_colors[BOARD_PIXEL_COUNT];

int main(void) {
    // Using seperate bool for fullscreen since if you toggle the fullscreen without setting window size first it messes with your monitor resolution
    bool fullscreen = false;

    srand(time(NULL));

    InitWindow(WINDOWED_SCREEN_WIDTH, WINDOWED_SCREEN_HEIGHT, "Hello!");

    // Enable vsync
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

    Texture2D board_state_texture = init_texture_with_size(BOARD_SIZE, BOARD_SIZE);
    Texture2D terrain_texture = init_texture_with_size(BOARD_SIZE, BOARD_SIZE);

    SetTextureFilter(board_state_texture, TEXTURE_FILTER_POINT);
    SetTextureFilter(terrain_texture, TEXTURE_FILTER_POINT);

    size_t map_state = MS_COUNTRY;

    int generation_seed = rand();

    gen_heightmap(generation_seed, BOARD_SIZE, heightmap);
    gen_terrain_image(0.33f, BOARD_SIZE, heightmap, terrain_pixel_colors);
    UpdateTexture(terrain_texture, terrain_pixel_colors);

    init_camera();
    init_map();

    // At the moment the order of initialization before here for everything is irrelevant

    set_pixel_state(get_index((Vector2) { 0, 0 }), RED, Fade(RED, 0.5f), Fade(RED, 0.25f), (PixelId) {
        1, 1, 1
    });
    set_pixel_state(get_index((Vector2) { 10, 10 }), ORANGE, Fade(ORANGE, 0.5f), Fade(ORANGE, 0.25f), (PixelId) {
        1, 1, 1
    });
    set_pixel_state(get_index((Vector2) { 10, 5 }), YELLOW, Fade(YELLOW, 0.5f), Fade(YELLOW, 0.25f), (PixelId) {
        1, 1, 1
    });

    clock_t render_clock_cycles = 0;
    
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
            gen_heightmap(generation_seed, BOARD_SIZE, heightmap);
            gen_terrain_image(0.33f, BOARD_SIZE, heightmap, terrain_pixel_colors);
            UpdateTexture(terrain_texture, terrain_pixel_colors);
        }

        update_camera();

        // Doesn't work on Windows because clock() is broken, perhaps we just scrap this whole frame-time thing?
        clock_t render_start_clock = clock();
        //! WARN: May need to enter texture mode if we add more textures in the future
        UpdateTexture(board_state_texture, map.colors[map_state]);

        BeginDrawing();

            BeginMode2D(camera);
                ClearBackground(BLACK);

                DrawTexturePro(board_state_texture, (Rectangle) { 0.0f, 0.0f, board_state_texture.width, board_state_texture.height }, (Rectangle) { 0.0f, 0.0f, BOARD_RECT_SIZE, BOARD_RECT_SIZE }, (Vector2) { 0.0f, 0.0f }, 0.0f, WHITE);
                DrawTextureEx(terrain_texture, (Vector2) { BOARD_SIZE * -1.5f, 0 }, 0.0f, 1.5f, WHITE);

            EndMode2D();
            
            DrawText(TextFormat("RND: %ld", render_clock_cycles), 10, GetRenderHeight() - 50, 20, GREEN);

            DrawFPS(10, GetRenderHeight() - 25);

        EndDrawing();

        clock_t render_end_clock = clock();
        render_clock_cycles = render_end_clock - render_start_clock;
    }
}