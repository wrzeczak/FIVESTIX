//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Game Board Generation Functions
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "raylib.h"
#include <time.h> // time(NULL)
#include <stdlib.h> // rand()

#include "fastnoise.h"

#define FML_IMPL

Image gen_raw_noise(int size, int seed) {
    fnl_state noise = fnlCreateState();
    noise.noise_type = FNL_NOISE_PERLIN;
    noise.seed = seed;

    // cue the magic numbers - discovered while messing around with the fnl GUI
    noise.fractal_type = FNL_FRACTAL_FBM;
    noise.frequency = 0.005f;
    noise.octaves = 6;
    noise.gain = 0.40f;
    noise.weighted_strength = -0.70f;

    Image output = GenImageColor(size, size, BLACK);

    for(int y = 0; y < size; y++) {
        for(int x = 0; x < size; x++) {
            float noise_data = fnlGetNoise2D(&noise, x, y);
            noise_data += 1;
            noise_data /= 2.0f;
            ImageDrawPixel(&output, x, y, Fade(RED, noise_data));
        }
    }

    return output;
}

//! TODO: ideally, this would take in an image or bunch of data instead of re-generating the noise; because we're using perlin, it should be the same, but it's needless performance baggage
Image gen_oceans(int size, int seed, float ocean_threshold) {
    fnl_state noise = fnlCreateState();
    noise.noise_type = FNL_NOISE_PERLIN;
    noise.seed = seed;

    // cue the magic numbers - discovered while messing around with the fnl GUI
    noise.fractal_type = FNL_FRACTAL_FBM;
    noise.frequency = 0.005f;
    noise.octaves = 6;
    noise.gain = 0.40f;
    noise.weighted_strength = -0.70f;

    Image output = GenImageColor(size, size, BLACK);

    for(int y = 0; y < size; y++) {
        for(int x = 0; x < size; x++) {
            float noise_data = fnlGetNoise2D(&noise, x, y);
            noise_data += 1;
            noise_data /= 2.0f;
            if(noise_data > ocean_threshold)
                ImageDrawPixel(&output, x, y, Fade(GREEN, noise_data));
            else
                ImageDrawPixel(&output, x, y, Fade(BLUE, noise_data));
        }
    }

    return output;
}