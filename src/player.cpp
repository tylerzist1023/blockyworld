#include "player.h"
#include "rcamera.h"

#define CAMERA_MOVE_SPEED                               0.5f
#define CAMERA_ROTATION_SPEED                           0.03f

// Camera mouse movement sensitivity
#define CAMERA_MOUSE_MOVE_SENSITIVITY                   0.003f    // TODO: it should be independant of framerate
#define CAMERA_MOUSE_SCROLL_SENSITIVITY                 1.5f

#define CAMERA_ORBITAL_SPEED                            0.5f       // Radians per second


#define CAMERA_FIRST_PERSON_STEP_TRIGONOMETRIC_DIVIDER  8.0f
#define CAMERA_FIRST_PERSON_STEP_DIVIDER                30.0f
#define CAMERA_FIRST_PERSON_WAVING_DIVIDER              200.0f

// PLAYER (used by camera)
#define PLAYER_MOVEMENT_SENSITIVITY                     20.0f

void UpdateCameraModified(Camera *camera, int mode)
{
    Vector2 mousePositionDelta = GetMouseDelta();

    bool moveInWorldPlane = false;//((mode == CAMERA_FIRST_PERSON) || (mode == CAMERA_THIRD_PERSON));
    bool rotateAroundTarget = ((mode == CAMERA_THIRD_PERSON) || (mode == CAMERA_ORBITAL));
    bool lockView = ((mode == CAMERA_FIRST_PERSON) || (mode == CAMERA_THIRD_PERSON) || (mode == CAMERA_ORBITAL));
    bool rotateUp = (mode == CAMERA_FREE);

    if (mode == CAMERA_ORBITAL)
    {
        // Orbital can just orbit
        Matrix rotation = MatrixRotate(GetCameraUp(camera), CAMERA_ORBITAL_SPEED*GetFrameTime());
        Vector3 view = Vector3Subtract(camera->position, camera->target);
        view = Vector3Transform(view, rotation);
        camera->position = Vector3Add(camera->target, view);
    }
    else
    {
        // Camera rotation
        if (IsKeyDown(KEY_DOWN)) CameraPitch(camera, -CAMERA_ROTATION_SPEED, lockView, rotateAroundTarget, rotateUp);
        if (IsKeyDown(KEY_UP)) CameraPitch(camera, CAMERA_ROTATION_SPEED, lockView, rotateAroundTarget, rotateUp);
        if (IsKeyDown(KEY_RIGHT)) CameraYaw(camera, -CAMERA_ROTATION_SPEED, rotateAroundTarget);
        if (IsKeyDown(KEY_LEFT)) CameraYaw(camera, CAMERA_ROTATION_SPEED, rotateAroundTarget);
        // if (IsKeyDown(KEY_Q)) CameraRoll(camera, -CAMERA_ROTATION_SPEED);
        // if (IsKeyDown(KEY_E)) CameraRoll(camera, CAMERA_ROTATION_SPEED);

        CameraYaw(camera, -mousePositionDelta.x*CAMERA_MOUSE_MOVE_SENSITIVITY, rotateAroundTarget);
        CameraPitch(camera, -mousePositionDelta.y*CAMERA_MOUSE_MOVE_SENSITIVITY, lockView, rotateAroundTarget, rotateUp);

        // Camera movement
        if (IsKeyDown(KEY_W)) CameraMoveForward(camera, CAMERA_MOVE_SPEED, moveInWorldPlane);
        if (IsKeyDown(KEY_A)) CameraMoveRight(camera, -CAMERA_MOVE_SPEED, moveInWorldPlane);
        if (IsKeyDown(KEY_S)) CameraMoveForward(camera, -CAMERA_MOVE_SPEED, moveInWorldPlane);
        if (IsKeyDown(KEY_D)) CameraMoveRight(camera, CAMERA_MOVE_SPEED, moveInWorldPlane);
        //if (IsKeyDown(KEY_SPACE)) CameraMoveUp(camera, CAMERA_MOVE_SPEED);
        //if (IsKeyDown(KEY_LEFT_CONTROL)) CameraMoveUp(camera, -CAMERA_MOVE_SPEED);
    }

    if ((mode == CAMERA_THIRD_PERSON) || (mode == CAMERA_ORBITAL))
    {
        // Zoom target distance
        CameraMoveToTarget(camera, -GetMouseWheelMove());
        if (IsKeyPressed(KEY_KP_SUBTRACT)) CameraMoveToTarget(camera, 2.0f);
        if (IsKeyPressed(KEY_KP_ADD)) CameraMoveToTarget(camera, -2.0f);
    }
}

void player_init(Player *player)
{
    player->camera.position = {0.f, 130.f, 0.f};
    player->camera.target = {0.f, 130.f, 1.f};
    player->camera.up = {0.f, 1.f, 0.f};
    player->camera.fovy = 60.f;
    player->camera.projection = CAMERA_PERSPECTIVE;
}

void player_update(Player *player)
{
    UpdateCameraModified(&(player->camera), CAMERA_FIRST_PERSON);
}