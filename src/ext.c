#include "ext.h"
#include "rlgl.h"
#include <stdlib.h>

Texture2D init_texture_with_size(int width, int height) {
    return (Texture2D) {
        .id = rlLoadTexture(NULL, width, height, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, 1),
        .width = width,
        .height = height,
        .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
        .mipmaps = 1
    };
}