//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Game Board Generation Functions
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "raylib.h"
#include <time.h> // time(NULL)
#include <stdlib.h> // rand()

#include "fastnoise.h"

#define FML_IMPL

void gen_heightmap(int seed, size_t size, float * heightmap) {
    fnl_state noise = fnlCreateState();
    noise.noise_type = FNL_NOISE_PERLIN;
    noise.seed = seed;

    // cue the magic numbers - discovered while messing around with the fnl GUI
    noise.fractal_type = FNL_FRACTAL_FBM;
    noise.frequency = 0.005f;
    noise.octaves = 6;
    noise.gain = 0.40f;
    noise.weighted_strength = -0.70f;

    size_t i = 0;
    for(size_t y = 0; y < size; y++) {
        for(size_t x = 0; x < size; x++, i++) {
            float height = fnlGetNoise2D(&noise, x, y);
            height += 1;
            height /= 2.0f;

            heightmap[i] = height;
        }
    }
}

void gen_terrain_image(float ocean_threshold, size_t size, const float * restrict heightmap, Color * pixel_colors) {
    size_t pixel_count = size*size;

    for(size_t i = 0; i < pixel_count; i++) {
        //! TODO: If the compiler can't optimize this branch well we may have to do some fuckery
        float height = heightmap[i];
        if(height > ocean_threshold) {
            pixel_colors[i] = Fade(GREEN, height);
        } else {
            pixel_colors[i] = Fade(BLUE, height);
        }
    }
}