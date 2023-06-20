#include "board.h"
#include "pixel_attrib.h"
#include "consts.h"
#include "camera.h"
#include "ext.h"
#include "fullscreen.h"
#include "pixel_dialog.h"
#include "temp_init.h"

#include "raylib.h"
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdalign.h>

int main(void) {
    //! TODO: Refactor due for main, we should move a lot of this stuff into their own files because main is very cluttered right now

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
    update_board_terrain(generation_seed, 0.5f);
    clock_t generation_end_clock = clock();

    clock_t generation_clock_cycles = generation_end_clock - generation_start_clock;

    size_t total_generation_count = 1;
    clock_t total_generation_clock_cycles = generation_clock_cycles;

    UpdateTexture(terrain_texture, board.terrain_pixel_colors);

    init_camera();
    init_board();

    // At the moment the order of initialization before here for everything is irrelevant

    temp_init_game();

    clock_t render_clock_cycles = 0;
    
    while(!WindowShouldClose()) {

        // Handle inputs

        update_fullscreen();

        if(IsKeyPressed(KEY_SPACE)) {
            map_state++;
            map_state %= 3;

            generation_seed++;
            generation_start_clock = clock();
            update_board_terrain(generation_seed, 0.33f);
            generation_end_clock = clock();
            generation_clock_cycles = generation_end_clock - generation_start_clock;
            total_generation_clock_cycles += generation_clock_cycles;
            total_generation_count++;

            UpdateTexture(terrain_texture, board.terrain_pixel_colors);
        }

        // Not sure whether to handle mouseover before or after camera position update, putting it here for now
        update_pixel_dialog();

        update_camera();

        //

        // Doesn't work on Windows because clock() is broken, perhaps we just scrap this whole frame-time thing?
        clock_t render_start_clock = clock();
        //! WARN: May need to enter texture mode if we add more textures in the future
        //! NOTE: Pretty confident above comment is not an issue
        UpdateTexture(map_texture, board.map_pixel_colors_arrays[map_state]);

        BeginDrawing();

            BeginMode2D(camera);
                ClearBackground(BLACK);

                DrawTexturePro(map_texture, (Rectangle) { 0.0f, 0.0f, map_texture.width, map_texture.height }, (Rectangle) { 0.0f, 0.0f, BOARD_RECT_SIZE, BOARD_RECT_SIZE }, (Vector2) { 0.0f, 0.0f }, 0.0f, WHITE);
                DrawTextureEx(terrain_texture, (Vector2) { BOARD_SIZE * -1.5f, 0 }, 0.0f, 1.5f, WHITE);
                
            EndMode2D();
            
            DrawText(TextFormat("T_GEN: %ld, %ld", total_generation_clock_cycles, total_generation_count), 10, GetRenderHeight() - 90, 20, GREEN);
            DrawText(TextFormat("GEN: %ld", generation_clock_cycles), 10, GetRenderHeight() - 70, 20, GREEN);
            DrawText(TextFormat("RND: %ld", render_clock_cycles), 10, GetRenderHeight() - 50, 20, GREEN);

            DrawFPS(10, GetRenderHeight() - 25);

        EndDrawing();

        clock_t render_end_clock = clock();
        render_clock_cycles = render_end_clock - render_start_clock;
    }
}