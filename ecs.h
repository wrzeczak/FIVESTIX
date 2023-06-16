#include "consts.h"

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  big game ECS 
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// typedef struct {
//     unsigned short id;
//     Color color;
//     //char * name;
// } Country;

// typedef struct {
//     unsigned short id;
//     Color color;
//     // int control_map[PIXEL_COUNT * PIXEL_COUNT];
//     //char * name;
// } Language;

// typedef struct {
//     unsigned short id;
//     Color color;
//     //char * name;
// } Culture;

// typedef struct {
//     Country country;
//     Culture culture;
//     Language language;
// } PixelState;

typedef enum {
    MS_COUNTRY,
    MS_CULTURE,
    MS_LANGUAGE
} MapStates;

typedef struct {
    unsigned short country_id;
    unsigned short culture_id;
    unsigned short language_id;
} PixelId;

// Using SoA here as to cut down on cacheline usage
// All data is indexed in row-major order, i.e x then y
typedef struct {
    // One array of colors (a texture in the future?) for each map state
    _Alignas(64) Color colors[MAP_STATES_COUNT][MAP_SIZE];
    // Pixel id information, maybe split into seperate arrays in the future if needed
    _Alignas(64) PixelId ids[MAP_SIZE];
} PixelStates;

// TODO: Link this properly
// Global variable used because theres only one map
static PixelStates map;

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// pixel functions
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void set_pixel_state(int index, Color country_color, Color culture_color, Color language_color, PixelId id) {
    map.colors[MS_COUNTRY][index] = country_color;
    map.colors[MS_CULTURE][index] = culture_color;
    map.colors[MS_LANGUAGE][index] = language_color;
    map.ids[index] = id;
}

void draw_map(int map_state) {
    // TODO: Maybe rewrite this function to use opengl calls
    const Color * pixel_colors = map.colors[map_state];

    int i = 0;
    for(int y = 0; y < PIXEL_COUNT; y++) {
        for(int x = 0; x < PIXEL_COUNT; x++, i++) {
            DrawRectangle(x*PIXEL_SIZE, y*PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE, pixel_colors[i]);
        }
    }
}