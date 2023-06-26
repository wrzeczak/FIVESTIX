#include "ext.h"
#include "rlgl.h"
#include <stdlib.h>
#include <GL/glew.h>

Texture2D init_texture_with_size(int width, int height) {
    return (Texture2D) {
        .id = rlLoadTexture(NULL, width, height, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, 1),
        .width = width,
        .height = height,
        .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
        .mipmaps = 1
    };
}

uint init_uvec3_texture(int width, int height) {
    uint texture_id;
    glGenTextures(1, &texture_id);
    rlEnableTexture(texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32UI, width, height, 0, GL_RGB_INTEGER, GL_UNSIGNED_INT, NULL);
    rlDisableTexture();
    return texture_id;
}

void read_uvec3_texture_pixels(uint texture_id, int width, int height, void* buffer) {
    rlEnableTexture(texture_id);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB_INTEGER, GL_UNSIGNED_INT, buffer);
    rlDisableTexture();
}

void read_texture_pixels(uint texture_id, int width, int height, int format, void* buffer) {
    rlEnableTexture(texture_id);
    uint gl_internal_format;
    uint gl_format;
    uint gl_type;
    rlGetGlTextureFormats(RL_PIXELFORMAT_UNCOMPRESSED_R32, &gl_internal_format, &gl_format, &gl_type);
    glGetTexImage(GL_TEXTURE_2D, 0, gl_format, gl_type, buffer);
    rlDisableTexture();
}