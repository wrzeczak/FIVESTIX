#include "temp_init.h"
#include "pixel_attrib.h"
#include "board.h"

static size_t get_index(Vector2 pos) {
    return (pos.y * BOARD_SIZE) + pos.x;
}

void temp_init_game(void) {
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
}