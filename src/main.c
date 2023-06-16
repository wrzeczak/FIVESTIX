#include "raylib.h"

#define SUPPORT_MODULE_RTEXTURES
#define SUPPORT_IMAGE_GENERATION

#include "ecs.h"
#include "board.h"
#include "consts.h"
#include "camera.h"

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

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello!");

    ToggleFullscreen();

    // enable vsync
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

    // TODO: Load this without discarding a bunch of stuff
    Texture2D board_texture = LoadRenderTexture(X_PIXELS_COUNT, Y_PIXELS_COUNT).texture;
    SetTextureFilter(board_texture, TEXTURE_FILTER_POINT);

    size_t map_state = MS_COUNTRY;

    Image perlin_noise_image;

    // perlin_noise_image = GenImagePerlinNoise(BOARD_SIZE, BOARD_SIZE, 0, 0, 1.0f);

    // Texture2D noise = LoadTextureFromImage(perlin_noise_image);

    init_map();

    set_pixel_state(get_index((Vector2) {0 , 0 }), RED, Fade(RED, 0.5f), Fade(RED, 0.25f), (PixelId) {
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

        if(IsKeyPressed(KEY_SPACE)) {
            map_state++;
            map_state %= 3;
        }

        update_camera();

        clock_t render_start_clock = clock();
        // WARN: May need to enter texture mode if we add more textures in the future
        UpdateTexture(board_texture, map.colors[map_state]);

        // replaced by TextFormat() -- also, doesn't work on Windows because clock() is broken, perhaps we just scrap this whole frame-time thing?
        // char render_text[16];
        // snprintf(render_text, 16, "RND: %ld", render_clock_cycles);

        BeginDrawing();

            BeginMode2D(camera);

                ClearBackground(BLACK);

                DrawTexturePro(board_texture, (Rectangle) { 0.0f, 0.0f, board_texture.width, board_texture.height },
                    (Rectangle) { 0.0f, 0.0f, BOARD_WIDTH, BOARD_HEIGHT }, (Vector2) { 0.0f, 0.0f }, 0.0f, WHITE);

            EndMode2D();

            DrawText(TextFormat("RND: %ld", render_clock_cycles), 10, SCREEN_HEIGHT - 50, 20, GREEN);

            DrawFPS(10, SCREEN_HEIGHT - 25);

        EndDrawing();

        clock_t render_end_clock = clock();
        render_clock_cycles = render_end_clock - render_start_clock;
    }
}