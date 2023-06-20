#include "pixel_dialog.h"
#include "consts.h"
#include "camera.h"
#include "board.h"
#include "pixel_attrib.h"
#include "raylib.h"
#include "raymath.h"
#include <stddef.h>

PixelDialog get_pixel_dialog(void) {
    Vector2 world_mouse_pos = Vector2Add(GetMousePosition(), camera.target);

    if(world_mouse_pos.x < 0.0f || world_mouse_pos.y < 0.0f) {
        return (PixelDialog) { NULL };
    }

    world_mouse_pos.x /= BOARD_PIXEL_SIZE;
    world_mouse_pos.y /= BOARD_PIXEL_SIZE;
    size_t x = world_mouse_pos.x;
    size_t y = world_mouse_pos.y;

    if(x >= BOARD_SIZE || y >= BOARD_SIZE) {
        return (PixelDialog) { NULL };
    }

    BoardPixelId id = board.ids[GET_BOARD_INDEX(x, y)];

    //! TODO: Probably should implement bounds checks here so we never read erroneous memory
    return (PixelDialog) {
        .country_display_name = id.country_id != NULL_USHORT ? country_display_names[id.country_id] : "None",
        .culture_display_name = id.culture_id != NULL_USHORT ? country_display_names[id.culture_id] : "None",
        .language_display_name = id.language_id != NULL_USHORT ? country_display_names[id.language_id] : "None"
    };
}