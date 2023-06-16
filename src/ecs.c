#include "ecs.h"
#include <stdalign.h>

_Alignas(64) PixelStates map;

void set_pixel_state(size_t index, Color country_color, Color culture_color, Color language_color, PixelId id) {
    map.colors[MS_COUNTRY][index] = country_color;
    map.colors[MS_CULTURE][index] = culture_color;
    map.colors[MS_LANGUAGE][index] = language_color;
    map.ids[index] = id;
}