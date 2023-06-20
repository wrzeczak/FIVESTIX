#include "render.h"
#include "camera.h"
#include "consts.h"
#include "ext.h"
#include <stdalign.h>

//! TODO: Like in camera.c we can use better types possibly
_Alignas(64) Texture2D board_map_texture;
Texture2D board_terrain_texture;

void init_render(void) {
    board_map_texture = init_texture_with_size(BOARD_SIZE, BOARD_SIZE);
    board_terrain_texture = init_texture_with_size(BOARD_SIZE, BOARD_SIZE);

    SetTextureFilter(board_map_texture, TEXTURE_FILTER_POINT);
    SetTextureFilter(board_terrain_texture, TEXTURE_FILTER_POINT);
}

void render_game(clock_t total_generation_clock_cycles, clock_t total_generation_count, clock_t generation_clock_cycles, clock_t render_clock_cycles) {
    BeginDrawing();

        BeginMode2D(camera);
            ClearBackground(BLACK);

            DrawTexturePro(board_map_texture, (Rectangle) { 0.0f, 0.0f, board_map_texture.width, board_map_texture.height }, (Rectangle) { 0.0f, 0.0f, BOARD_RECT_SIZE, BOARD_RECT_SIZE }, (Vector2) { 0.0f, 0.0f }, 0.0f, WHITE);
            DrawTextureEx(board_terrain_texture, (Vector2) { BOARD_SIZE * -1.5f, 0 }, 0.0f, 1.5f, WHITE);
            
        EndMode2D();
        
        DrawText(TextFormat("T_GEN: %ld, %ld", total_generation_clock_cycles, total_generation_count), 10, GetRenderHeight() - 90, 20, GREEN);
        DrawText(TextFormat("GEN: %ld", generation_clock_cycles), 10, GetRenderHeight() - 70, 20, GREEN);
        DrawText(TextFormat("RND: %ld", render_clock_cycles), 10, GetRenderHeight() - 50, 20, GREEN);

        DrawFPS(10, GetRenderHeight() - 25);

    EndDrawing();
}