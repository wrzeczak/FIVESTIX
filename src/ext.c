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

//! TODO: Find a proper solution for this
// Ugly hack, rlgl uses a heap allocated buffer on its rlReadTexturePixels function and we want to read into a predetermined buffer, hence this is necessary
void glGetTexImage(uint target, int level, uint format, uint type, void * pixels);
#define GL_TEXTURE_2D 0x0DE1
//

void read_texture_pixels(uint texture_id, int width, int height, int format, void* buffer) {
    rlEnableTexture(texture_id);
    uint gl_internal_format;
    uint gl_format;
    uint gl_type;
    rlGetGlTextureFormats(RL_PIXELFORMAT_UNCOMPRESSED_R32, &gl_internal_format, &gl_format, &gl_type);
    glGetTexImage(GL_TEXTURE_2D, 0, gl_format, gl_type, buffer);
    rlDisableTexture();
}