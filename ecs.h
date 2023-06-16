#include "consts.h"
#include <GL\glew.h>

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
} MapState;

typedef struct {
    unsigned short country_id;
    unsigned short culture_id;
    unsigned short language_id;
} PixelId;

// Using SoA here as to cut down on cacheline usage
// All data is indexed in row-major order, i.e x then y
typedef struct {
    // One array of colors (a texture in the future?) for each map state
    _Alignas(64) Color colors[MAP_STATES_COUNT][PIXELS_COUNT];
    // Pixel id information, maybe split into seperate arrays in the future if needed
    _Alignas(64) PixelId ids[PIXELS_COUNT];
} PixelStates;

// TODO: Link this properly
// Global variable used because theres only one map
static PixelStates map;

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// pixel functions
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void set_pixel_state(size_t index, Color country_color, Color culture_color, Color language_color, PixelId id) {
    map.colors[MS_COUNTRY][index] = country_color;
    map.colors[MS_CULTURE][index] = culture_color;
    map.colors[MS_LANGUAGE][index] = language_color;
    map.ids[index] = id;
}

void draw_map(size_t map_state, Texture2D* tex) {
    glBindTexture(GL_TEXTURE_2D, tex->id);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, X_PIXELS_COUNT, Y_PIXELS_COUNT, GL_RGBA, GL_UNSIGNED_BYTE, map.colors[map_state]);
}