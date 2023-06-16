#include "raylib.h"

#include "ecs.h"
#include "board.h"

#include "consts.h"

#include <stdarg.h>
#include <stdlib.h>

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

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello!");

    ToggleFullscreen();

    SetTargetFPS(75);

    RenderTexture2D board = LoadRenderTexture(GAME_SIZE, GAME_SIZE);
    SetTextureFilter(board.texture, TEXTURE_FILTER_BILINEAR);

    int map_state = MS_COUNTRY;

    PixelState map[PIXEL_COUNT*PIXEL_COUNT];

    for(int i = 0; i < MAP_SIZE; i++) {
        map[i] = (PixelState) { NULL, NULL, NULL };
    }

    Country red_country = (Country) { 1, RED };
    Country orange_country = (Country) { 1, ORANGE };
    Country yellow_country = (Country) { 1, YELLOW };

    Culture red_culture = (Culture) { 1, Fade(RED, 0.5f) };
    Culture orange_culture = (Culture) { 1, Fade(ORANGE, 0.5f) };
    Culture yellow_culture = (Culture) { 1, Fade(YELLOW, 0.5f) };

    Language red_langugage = (Language) { 1, Fade(RED, 0.25f) };
    Language orange_language = (Language) { 1, Fade(ORANGE, 0.25f) };
    Language yellow_language = (Language) { 1, Fade(YELLOW, 0.25f) };

    PixelState red_pixel = (PixelState) { red_country, red_culture, red_langugage };
    PixelState orange_pixel = (PixelState) { orange_country, orange_culture, orange_language };
    PixelState yellow_pixel = (PixelState) { yellow_country, yellow_culture, yellow_language };

    set_pixel_state(map, red_pixel, (Vector2) {0 , 0 });
    set_pixel_state(map, orange_pixel, (Vector2) { 10, 10 });
    set_pixel_state(map, yellow_pixel, (Vector2) { 10, 5 });
    
    while(!WindowShouldClose()) {

        if(IsKeyPressed(KEY_SPACE)) {
            map_state++;
        }

        BeginTextureMode(board);
            ClearBackground(RAYWHITE);

            draw_map(map, map_state % 3);

        EndTextureMode();

        BeginDrawing();

            ClearBackground(BLACK);

            DrawTexturePro(board.texture, (Rectangle){ 0.0f, 0.0f, board.texture.width, -board.texture.height },
                (Rectangle){ X_OFFSET, Y_OFFSET, GAME_SIZE, GAME_SIZE}, (Vector2){ 0, 0 }, 0.0f, WHITE);

            DrawFPS(10, SCREEN_HEIGHT - 25);

        EndDrawing();
    }
}