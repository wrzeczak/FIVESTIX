#pragma once
#include "consts.h"
#include "raylib.h"
#include <stdlib.h>

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

// Global variable used because theres only one map
extern PixelStates map;

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// pixel functions
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void set_pixel_state(size_t index, Color country_color, Color culture_color, Color language_color, PixelId id);