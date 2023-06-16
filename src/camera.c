#include "camera.h"
#include "consts.h"
#include "raymath.h"

#define MOVE_SPEED 25.0f

Camera2D camera;
static Vector2 camera_velocity = { 0.0f, 0.0f };

// Not really necessary could be initialized globally
void init_camera(void) {
    camera.target = (Vector2) { 0.0f, 0.0f };
    camera.offset = (Vector2) { 0.0f, 0.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
}

void update_camera(void) {
    Vector2 input_vec = { 0.0f, 0.0f };
    if(IsKeyDown(KEY_W)) {
        input_vec.y -= 1.0f;
    }
    if(IsKeyDown(KEY_S)) {
        input_vec.y += 1.0f;
    }
    if(IsKeyDown(KEY_D)) {
        input_vec.x += 1.0f;
    }
    if(IsKeyDown(KEY_A)) {
        input_vec.x -= 1.0f;
    }

    if(input_vec.x == 0.0f && input_vec.y == 0.0f) {
        input_vec = Vector2Normalize(input_vec);
    }
    
    Vector2 move_vec = Vector2Scale(input_vec, MOVE_SPEED);
    camera_velocity = Vector2Lerp(camera_velocity, move_vec, 0.2f);
    camera.target = Vector2Add(camera.target, camera_velocity);
}