#include "raylib.h"

#include "ecs.h"
#include "board.h"

#include "consts.h"

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>

/*
    struct {
        colors[3];
        ...
    } Pixel;

    int map_state = MS_COUNTRY

    if (spacebar has been pressed) {
        map_state++;
    }
    draw the texture {
        draw rect ... pixel.color[map_state % 3];
    }

*/

// testing VSCode github integration...

int get_index(Vector2 pos) {
    return (pos.y * X_PIXELS_COUNT) + pos.x;
}

void init_map() {
    memset(&map.ids, 0, sizeof(map.ids));

    for (int i = 0; i < MAP_STATES_COUNT; i++) {
        Color * pixel_colors = map.colors[i];
        for (int j = 0; j < PIXELS_COUNT; j++) {
            pixel_colors[j] = WHITE;
        }
    }
}

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello!");

    ToggleFullscreen();

    SetTargetFPS(75);

    RenderTexture2D board = LoadRenderTexture(X_PIXELS_COUNT, Y_PIXELS_COUNT);
    SetTextureFilter(board.texture, TEXTURE_FILTER_POINT);

    int map_state = MS_COUNTRY;

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
    
    while(!WindowShouldClose()) {

        if(IsKeyPressed(KEY_SPACE)) {
            map_state++;
            map_state %= 3;
        }

        clock_t render_start_clock = clock();
        draw_map(map_state, &board);
        clock_t render_end_clock = clock();
        clock_t render_clock_cycles = render_end_clock - render_start_clock;

        char render_text[16];
        snprintf(render_text, 16, "RND: %ld", render_clock_cycles);

        BeginDrawing();

            ClearBackground(BLACK);

            DrawTexturePro(board.texture, (Rectangle){ 0.0f, 0.0f, board.texture.width, board.texture.height },
                (Rectangle){ X_OFFSET, Y_OFFSET, BOARD_WIDTH, BOARD_HEIGHT }, (Vector2){ 0, 0 }, 0.0f, WHITE);

            DrawText(render_text, 10, SCREEN_HEIGHT - 50, 20, GREEN);

            DrawFPS(10, SCREEN_HEIGHT - 25);

        EndDrawing();
    }
}