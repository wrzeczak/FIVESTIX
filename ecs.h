#include "consts.h"

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  big game ECS 
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

typedef struct {
    unsigned short id;
    Color color;
    //char * name;
} Country;

typedef struct {
    unsigned short id;
    Color color;
    // int control_map[PIXEL_COUNT * PIXEL_COUNT];
    //char * name;
} Language;

typedef struct {
    unsigned short id;
    Color color;
    //char * name;
} Culture;

typedef struct {
    Country country;
    Culture culture;
    Language language;
} PixelState;

typedef enum {
    MS_COUNTRY,
    MS_CULTURE,
    MS_LANGUAGE
} MapStates;

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// pixel functions
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void set_pixel_state(PixelState * map, PixelState px, Vector2 pos) {
    int idx = pos.y * PIXEL_COUNT + pos.x;
    map[idx] = px;
}

void draw_map(PixelState * map, int map_state) {
    for(int i = 0; i < MAP_SIZE; i++) {
        int x = (i % PIXEL_COUNT) * PIXEL_SIZE;
        int y = ((i -(i % PIXEL_COUNT)) / PIXEL_COUNT) * PIXEL_SIZE; // paranoia ensues
        switch(map_state) {
            case MS_COUNTRY:
                DrawRectangle(x, y, PIXEL_SIZE, PIXEL_SIZE, map->country.color);
                break;
            case MS_CULTURE:
                DrawRectangle(x, y, PIXEL_SIZE, PIXEL_SIZE, map->culture.color);
                break;
            case MS_LANGUAGE:
                DrawRectangle(x, y, PIXEL_SIZE, PIXEL_SIZE, map->language.color);
                break;
        }

        map++;
    }
}