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

size_t get_index(Vector2 pos) {
    return (pos.y * X_PIXELS_COUNT) + pos.x;
}

void init_map() {
    memset(&map.ids, 0, sizeof(map.ids));

    for(size_t i = 0; i < MAP_STATES_COUNT; i++) {
        Color * pixel_colors = map.colors[i];
        for(size_t j = 0; j < PIXELS_COUNT; j++) {
            pixel_colors[j] = WHITE;
        }
    }
}

#define WINDOWED_SCREEN_WIDTH 1280
#define WINDOWED_SCREEN_HEIGHT 720

int main(void) {
    // Using seperate bool for fullscreen since if you toggle the fullscreen without setting window size first it messes with your monitor resolution
    bool fullscreen = false;

    InitWindow(WINDOWED_SCREEN_WIDTH, WINDOWED_SCREEN_HEIGHT, "Hello!");

    // Enable vsync
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

    Texture2D board_texture = init_texture_with_size(X_PIXELS_COUNT, Y_PIXELS_COUNT);
    SetTextureFilter(board_texture, TEXTURE_FILTER_POINT);

    size_t map_state = MS_COUNTRY;

    Image perlin_noise_image;

    //! TODO: Make our own perlin noise algorithm or use another library, as this uses the heap along with not being able to set a seed
    //! NOTE: This uses the heap
    perlin_noise_image = GenImagePerlinNoise(BOARD_SIZE, BOARD_SIZE, 0, 0, 1.0f);
    Texture2D noise = LoadTextureFromImage(perlin_noise_image);

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
            if (fullscreen) {
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
        }

        update_camera();

        // Doesn't work on Windows because clock() is broken, perhaps we just scrap this whole frame-time thing?
        clock_t render_start_clock = clock();
        //! WARN: May need to enter texture mode if we add more textures in the future
        UpdateTexture(board_texture, map.colors[map_state]);

        BeginDrawing();

            BeginMode2D(camera);
                ClearBackground(BLACK);

                DrawTexturePro(board_texture, (Rectangle) { 0.0f, 0.0f, board_texture.width, board_texture.height }, (Rectangle) { 0.0f, 0.0f, BOARD_WIDTH, BOARD_HEIGHT }, (Vector2) { 0.0f, 0.0f }, 0.0f, WHITE);
            EndMode2D();

            DrawTexture(noise, 100, 100, WHITE);
            
            DrawText(TextFormat("RND: %ld", render_clock_cycles), 10, GetRenderHeight() - 50, 20, GREEN);

            DrawFPS(10, GetRenderHeight() - 25);

        EndDrawing();

        clock_t render_end_clock = clock();
        render_clock_cycles = render_end_clock - render_start_clock;
    }
}