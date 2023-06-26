#include "pixel_attrib.h"
#include "consts.h"
#include <stdalign.h>

_Alignas(64) Vector4 country_colors[MAX_COUNTRIES_COUNT];

_Alignas(64) const char * country_display_names[MAX_COUNTRIES_COUNT];
_Alignas(64) const char * culture_display_names[MAX_COUNTRIES_COUNT];
_Alignas(64) const char * language_display_names[MAX_COUNTRIES_COUNT];