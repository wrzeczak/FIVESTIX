#include "board_gen.h"
#include "consts.h"
#include "types.h"
#include "render.h"
#include "raylib.h"
#include "rlgl.h"
#include <stddef.h>

_Alignas(64) static Shader shader;
static uint framebuffer;

//! TODO: Find a proper solution for this
// Ugly hack, rlgl uses a heap allocated buffer on its rlReadTexturePixels function and we want to read into a predetermined buffer, hence this is necessary
void glGetTexImage(uint target, int level, uint format, uint type, void* pixels);
#define GL_TEXTURE_2D 0x0DE1
//

void init_board_gen(void) {
    shader = LoadShader(0, "resources/board_gen_fragment.glsl");
    
    // Params are unused here for some reason
    framebuffer = rlLoadFramebuffer(BOARD_SIZE, BOARD_SIZE);

    rlFramebufferAttach(framebuffer, board_terrain_texture.id, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_TEXTURE2D, 0);
}

void generate_board(void) {
    rlDisableColorBlend();
    BeginTextureMode((RenderTexture2D) { .texture = { .width = BOARD_SIZE, .height = BOARD_SIZE } });
    BeginShaderMode(shader);
    rlEnableFramebuffer(framebuffer);
    rlBegin(RL_QUADS);

    rlColor4ub(255, 255, 255, 255);
    rlNormal3f(0.0f, 0.0f, 1.0f);                          // Normal vector pointing towards viewer

    // Top-left corner for texture and quad
    rlTexCoord2f(0.0f, 1.0f);
    rlVertex2f(0, 0);

    // Bottom-left corner for texture and quad
    rlTexCoord2f(0.0f, 0.0f);
    rlVertex2f(0, BOARD_SIZE);

    // Bottom-right corner for texture and quad
    rlTexCoord2f(1.0f, 0.0f);
    rlVertex2f(BOARD_SIZE, BOARD_SIZE);

    // Top-right corner for texture and quad
    rlTexCoord2f(1.0f, 1.0f);
    rlVertex2f(BOARD_SIZE, 0);

    rlEnd();
    EndShaderMode();
    EndTextureMode();
    rlDisableFramebuffer();
    rlEnableColorBlend();
}