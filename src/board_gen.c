#include "board_gen.h"
#include "consts.h"
#include "types.h"
#include "render.h"
#include "raylib.h"
#include "pixel_attrib.h"
#include "board.h"
#include "ext.h"
#include "rlgl.h"
#include "raymath.h"
#include <stddef.h>
#include <stdio.h>

_Alignas(64) static Shader shader;
static uint framebuffer_id;
static uint board_pixel_id_texture_id;

void init_board_gen(void) {
    board_pixel_id_texture_id = init_uvec3_texture(BOARD_SIZE, BOARD_SIZE);

    shader = LoadShader(NULL, "resources/board_gen_fragment.glsl");
    
    // Params are unused here for some reason
    framebuffer_id = rlLoadFramebuffer(BOARD_SIZE, BOARD_SIZE);

    rlFramebufferAttach(framebuffer_id, board_terrain_texture.id, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_TEXTURE2D, 0);
    rlFramebufferAttach(framebuffer_id, board_country_map_texture.id, RL_ATTACHMENT_COLOR_CHANNEL1, RL_ATTACHMENT_TEXTURE2D, 0);
    rlFramebufferAttach(framebuffer_id, board_pixel_id_texture_id, RL_ATTACHMENT_COLOR_CHANNEL2, RL_ATTACHMENT_TEXTURE2D, 0);
}

void generate_board(float seed) {
    SetShaderValueV(shader, GetShaderLocation(shader, "country_colors"), country_colors, SHADER_UNIFORM_VEC4, MAX_COUNTRIES_COUNT);
    SetShaderValue(shader, GetShaderLocation(shader, "gen_seed"), &seed, SHADER_UNIFORM_FLOAT);

    // This needs to be done for some reason, otherwise other color channels will not output
    rlEnableFramebuffer(framebuffer_id);

    rlActiveDrawBuffers(3);
    rlDisableColorBlend();
    BeginTextureMode((RenderTexture2D) { .texture = { .width = BOARD_SIZE, .height = BOARD_SIZE } });
    BeginShaderMode(shader);
    rlEnableFramebuffer(framebuffer_id);
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
    rlActiveDrawBuffers(1);

    read_uvec3_texture_pixels(board_pixel_id_texture_id, BOARD_SIZE, BOARD_SIZE, board.ids);
}