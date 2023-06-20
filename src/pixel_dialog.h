#pragma once

typedef struct {
    const char* country_display_name;
    const char* culture_display_name;
    const char* language_display_name;
} PixelDialog;

PixelDialog get_pixel_dialog(void);