#include "player.h"
#include "raymath.h"
#include "world.h"
#include "raylib.h "
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
    player->camera.position = {0.f, 64.f, 0.f};
    player->camera.target = {0.f, 64.f, 1.f};
    player->camera.up = {0.f, 1.f, 0.f};
    player->camera.fovy = 80.f;
    player->camera.projection = CAMERA_PERSPECTIVE;
}

void player_update(Player *player, World *world, Atlas atlas)
{
    UpdateCameraModified(&(player->camera), CAMERA_FIRST_PERSON);

    bool place_block = false;
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || (place_block=IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)))
    {
        Ray ray = GetMouseRay(GetMousePosition(), player->camera);

        for(int i = 0; i < world->chunks.size; i++)
        {
            // TODO: When we make the chunk generation multithreaded, remember that this Chunk pointer needs to be accounted for!
            // It doesn't matter if we copy this by value or not because of the chunk data.
            Chunk *our_chunk = &(world->chunks[i]);

            Matrix chunk_transform = MatrixTranslate(our_chunk->x * CHUNK_SIZE, 0.f, our_chunk->z * CHUNK_SIZE);
            RayCollision collision_mesh = GetRayCollisionMesh(ray, our_chunk->data->finished_model.meshes[0], chunk_transform);

            collision_mesh.point = Vector3Subtract(collision_mesh.point, Vector3Scale(collision_mesh.normal, place_block ? -.5f : .5f));

            if(collision_mesh.hit)
            {
                int blockx = (int)(collision_mesh.point.x - (our_chunk->x  * CHUNK_SIZE));
                int blocky = (int)(collision_mesh.point.y);
                int blockz = (int)(collision_mesh.point.z - (our_chunk->z * CHUNK_SIZE));

                // Fixes a bug where placing a block on the edge of a chunk wraps around the chunk.
                if(place_block)
                {
                    if(blockx < 0)
                    {
                        our_chunk = world_find_chunk(world, our_chunk->x-1, our_chunk->z);
                        blockx += CHUNK_SIZE;
                    }
                    else if(blockx >= CHUNK_SIZE)
                    {
                        our_chunk = world_find_chunk(world, our_chunk->x+1, our_chunk->z);
                        blockx -= CHUNK_SIZE;
                    }
                    else if(blockz < 0)
                    {
                        our_chunk = world_find_chunk(world, our_chunk->x, our_chunk->z-1);
                        blockz += CHUNK_SIZE;
                    }
                    else if(blockz >= CHUNK_SIZE)
                    {
                        our_chunk = world_find_chunk(world, our_chunk->x, our_chunk->z+1);
                        blockz -= CHUNK_SIZE;

                    }
                    if(!our_chunk)
                    {
                        break;
                    }
                }

                our_chunk->data->blocks[blocky][blockx][blockz].type = place_block ? BLOCK_TYPE_STONE : BLOCK_TYPE_AIR;
                chunk_update_model(our_chunk->data, atlas);
                world_update_neighbors(world, *our_chunk, atlas);

                break;
            }
        }
    }
}