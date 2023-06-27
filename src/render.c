#include "render.h"
#include "camera.h"
#include "consts.h"
#include "ext.h"
#include <stdalign.h>

//! TODO: Like in camera.c we can use better types possibly
_Alignas(64) Texture2D board_terrain_texture;
Texture2D board_country_map_texture;

void init_render(void) {
    board_terrain_texture = init_texture_with_size(BOARD_SIZE, BOARD_SIZE);
    board_country_map_texture = init_texture_with_size(BOARD_SIZE, BOARD_SIZE);

    SetTextureFilter(board_terrain_texture, TEXTURE_FILTER_POINT);
    SetTextureFilter(board_country_map_texture, TEXTURE_FILTER_POINT);
}

void render_game(double total_generation_duration, size_t total_generation_count, double generation_duration, double render_duration, PixelDialog pixel_dialog) {
    BeginDrawing();

        BeginMode2D(camera);
            ClearBackground(BLACK);

            // DrawTexturePro(board_country_map_texture, (Rectangle) { 0.0f, 0.0f, board_country_map_texture.width, board_country_map_texture.height }, (Rectangle) { 0.0f, 0.0f, BOARD_RECT_SIZE, BOARD_RECT_SIZE }, (Vector2) { 0.0f, 0.0f }, 0.0f, WHITE);
            // DrawTextureEx(board_terrain_texture, (Vector2) { BOARD_SIZE * -1.5f, 0 }, 0.0f, 1.5f, WHITE);
            DrawTextureEx(board_terrain_texture, (Vector2) { 0, 0 }, 0.0f, 1.5f, WHITE);
            DrawTextureEx(board_country_map_texture, (Vector2) { BOARD_SIZE * 1.5f, 0 }, 0.0f, 1.5f, WHITE);
            
        EndMode2D();
        
        if (pixel_dialog.country_display_name != NULL) {
            //! TODO: Bad reacquire, at least look at raylib implementation
            Vector2 mouse_pos = GetMousePosition();

            DrawText(TextFormat("Country: %s\nCulture: %s\nLanguage: %s", pixel_dialog.country_display_name, pixel_dialog.culture_display_name, pixel_dialog.language_display_name), mouse_pos.x, mouse_pos.y, 20, BLACK);
        }

        DrawText(TextFormat("T_GEN: %f, %ld", total_generation_duration, total_generation_count), 10, GetRenderHeight() - 90, 20, GREEN);
        DrawText(TextFormat("GEN: %f", generation_duration), 10, GetRenderHeight() - 70, 20, GREEN);
        DrawText(TextFormat("RND: %f", render_duration), 10, GetRenderHeight() - 50, 20, GREEN);

        DrawFPS(10, GetRenderHeight() - 25);

    EndDrawing();
}