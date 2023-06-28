#include "pixel_attrib.h"
#include "consts.h"
#include <stdalign.h>

_Alignas(64) Vector4 country_colors[MAX_COUNTRIES_COUNT];
_Alignas(64) Vector2 country_centers[MAX_COUNTRIES_COUNT];

_Alignas(64) const char * country_display_names[MAX_COUNTRIES_COUNT] = {
    "Country0",
    "Country1",
    "Country2",
    "Country3",
    "Country4",
    "Country5",
    "Country6",
    "Country7"
};
_Alignas(64) const char * culture_display_names[MAX_COUNTRIES_COUNT] = {
    "Culture0",
    "Culture1",
    "Culture2",
    "Culture3",
    "Culture4",
    "Culture5",
    "Culture6",
    "Culture7"
};
_Alignas(64) const char * language_display_names[MAX_COUNTRIES_COUNT] = {
    "Language0",
    "Language1",
    "Language2",
    "Language3",
    "Language4",
    "Language5",
    "Language6",
    "Language7"
};