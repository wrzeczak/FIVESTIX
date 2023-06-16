#include "raylib.h"

#include "ecs.h"
#include "board.h"

#include "consts.h"

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

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

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello!");

    ToggleFullscreen();

    SetTargetFPS(75);

    RenderTexture2D board = LoadRenderTexture(BOARD_WIDTH, BOARD_HEIGHT);
    SetTextureFilter(board.texture, TEXTURE_FILTER_BILINEAR);

    int map_state = MS_COUNTRY;

    // TODO: Maybe move to an init or something like that function
    memset(&map, 0, sizeof(map));

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

        BeginTextureMode(board);
            ClearBackground(RAYWHITE);

            draw_map(map_state);

        EndTextureMode();

        BeginDrawing();

            ClearBackground(BLACK);

            DrawTexturePro(board.texture, (Rectangle){ 0.0f, 0.0f, board.texture.width, -board.texture.height },
                (Rectangle){ X_OFFSET, Y_OFFSET, BOARD_WIDTH, BOARD_HEIGHT }, (Vector2){ 0, 0 }, 0.0f, WHITE);

            DrawFPS(10, SCREEN_HEIGHT - 25);

        EndDrawing();
    }
}