#include "raylib.h"

#include "board.h"
#include "pixel_attrib.h"
#include "consts.h"
#include "camera.h"
#include "ext.h"
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

int main(void) {
    //! TODO: Refactor due for main, we should move a lot of this stuff into their own files because main is very cluttered right now

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
    update_board_terrain(generation_seed, 0.5f);
    clock_t generation_end_clock = clock();

    clock_t generation_clock_cycles = generation_end_clock - generation_start_clock;

    size_t total_generation_count = 1;
    clock_t total_generation_clock_cycles = generation_clock_cycles;

    UpdateTexture(terrain_texture, board.terrain_pixel_colors);

    init_camera();
    init_board();

    // At the moment the order of initialization before here for everything is irrelevant

    country_display_names[0] = "TestCountry";
    culture_display_names[0] = "TestCulture";
    language_display_names[0] = "TestLanguage";

    set_board_map_pixel_state(get_index((Vector2) { 0, 0 }), RED, Fade(RED, 0.5f), Fade(RED, 0.25f), (BoardPixelId) {
        0, 0, 0
    });
    set_board_map_pixel_state(get_index((Vector2) { 10, 10 }), ORANGE, Fade(ORANGE, 0.5f), Fade(ORANGE, 0.25f), (BoardPixelId) {
        0, 0, 0
    });
    set_board_map_pixel_state(get_index((Vector2) { 10, 5 }), YELLOW, Fade(YELLOW, 0.5f), Fade(YELLOW, 0.25f), (BoardPixelId) {
        0, 0, 0
    });

    clock_t render_clock_cycles = 0;
    
    while(!WindowShouldClose()) {

        // Handle inputs

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
            total_generation_clock_cycles += generation_clock_cycles;
            total_generation_count++;

            UpdateTexture(terrain_texture, board.terrain_pixel_colors);
        }

        // Not sure whether to handle mouseover before or after camera position update, putting it here for now
        Vector2 world_mouse_pos = Vector2Add(GetMousePosition(), camera.target);
        if (world_mouse_pos.x >= 0.0f && world_mouse_pos.y >= 0.0f) {
            world_mouse_pos.x /= BOARD_PIXEL_SIZE;
            world_mouse_pos.y /= BOARD_PIXEL_SIZE;
            size_t x = world_mouse_pos.x;
            size_t y = world_mouse_pos.y;

            if (x < BOARD_SIZE && y < BOARD_SIZE) {
                BoardPixelId id = board.ids[(y * BOARD_SIZE) + x];

                // //! TODO: Probably should implement bounds checks here so we never read erroneous memory
                const char* country_display_name = id.country_id != NULL_USHORT ? country_display_names[id.country_id] : "None";
                const char* culture_display_name = id.culture_id != NULL_USHORT ? country_display_names[id.culture_id] : "None";
                const char* language_display_name = id.language_id != NULL_USHORT ? country_display_names[id.language_id] : "None";

                printf("Country: %s\nCulture: %s\nLanguage: %s\n", country_display_name, culture_display_name, language_display_name);
            }
        }

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