#include "board.h"
#include "consts.h"
#include "camera.h"
#include "fullscreen.h"
#include "pixel_dialog.h"
#include "temp_init.h"
#include "render.h"

#include "raylib.h"
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdalign.h>

//! TODO: Move a bunch of implementations of commonly used together variables to the same translation unit for better cache usage

int main(void) {
    // srand(time(NULL));

    InitWindow(WINDOWED_SCREEN_WIDTH, WINDOWED_SCREEN_HEIGHT, "Hello!");

    // Enable vsync
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

    init_render();

    size_t map_state = MS_COUNTRY;

    init_camera();
    init_board();

    // Don't do this until we actually want random terrain
    // int generation_seed = rand();
    int generation_seed = 0;

    double generation_start = GetTime();
    update_board_terrain(generation_seed);
    double generation_duration = GetTime() - generation_start;

    size_t total_generation_count = 1;
    double total_generation_duration = generation_duration;

    // At the moment the order of initialization before here for everything is irrelevant

    temp_init_game();

    double render_duration = 0.0;
    
    while(!WindowShouldClose()) {

        // Handle inputs

        update_fullscreen();

        if(IsKeyPressed(KEY_SPACE)) {
            map_state++;
            map_state %= 3;

            generation_seed++;
            generation_start = GetTime();
            update_board_terrain(generation_seed);
            generation_duration = GetTime() - generation_start;
            total_generation_duration += generation_duration;
            total_generation_count++;
        }

        update_camera();

        // Putting after camera update since it renders where the mouse is in the window
        PixelDialog pixel_dialog = get_pixel_dialog();

        //

        double render_start = GetTime();

        render_game(total_generation_duration, total_generation_count, generation_duration, render_duration, pixel_dialog);

        render_duration = GetTime() - render_start;
    }
}