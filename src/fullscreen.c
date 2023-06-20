#include "fullscreen.h"
#include "consts.h"
#include "raylib.h"
#include <stdbool.h>

// Using seperate bool for fullscreen since if you toggle the fullscreen without setting window size first it messes with your monitor resolution
static bool fullscreen = false;

void update_fullscreen(void) {
    // Ugly hack, unless you know how to make fullscreen work correctly do not touch this
    if(IsKeyPressed(KEY_F11)) {
        fullscreen = !fullscreen;
        int width;
        int height;
        if(fullscreen) {
            int monitor = GetCurrentMonitor();
            width = GetMonitorWidth(monitor);
            height = GetMonitorHeight(monitor);
        } else {
            width = WINDOWED_SCREEN_WIDTH;
            height = WINDOWED_SCREEN_HEIGHT;
        }
        // Doing this twice because it doesnt work if only done once
        SetWindowSize(width, height);
        ToggleFullscreen();
        SetWindowSize(width, height);
    }
}