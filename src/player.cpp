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

        // We need to look for the chunk that is closest to the player, or the raycast will collide with other chunks, where the raycast will have to travel through other chunks to reach, which is not what we want!
        Chunk *closest_chunk = 0;
        RayCollision closest_collision_mesh = {0};
        closest_collision_mesh.distance = INFINITY;

        for(int i = 0; i < world->chunks.size; i++)
        {
            // TODO: When we make the chunk generation multithreaded, remember that this Chunk pointer needs to be accounted for!
            // It doesn't matter if we copy this by value or not because of the chunk data.
            Chunk *our_chunk = &(world->chunks[i]);

            Matrix chunk_transform = MatrixTranslate(float(our_chunk->x) * CHUNK_SIZE, 0.f, float(our_chunk->z) * CHUNK_SIZE);
            RayCollision collision_mesh = GetRayCollisionMesh(ray, our_chunk->data->finished_model.meshes[0], chunk_transform);

            // We add the mesh's normal to the collision point so it falls on the inside of the block, so when we do the div, it will guarantee the correct block is selected.
            collision_mesh.point = Vector3Subtract(collision_mesh.point, Vector3Scale(collision_mesh.normal, place_block ? -.5f : .5f));

            if(collision_mesh.hit && collision_mesh.distance < closest_collision_mesh.distance)
            {
                closest_chunk = our_chunk;
                closest_collision_mesh = collision_mesh;
            }
        }

        if(closest_chunk)
        {
            int blockx = (int)(closest_collision_mesh.point.x - (closest_chunk->x  * CHUNK_SIZE));
            int blocky = (int)(closest_collision_mesh.point.y);
            int blockz = (int)(closest_collision_mesh.point.z - (closest_chunk->z * CHUNK_SIZE));

            // Fixes a bug where placing a block on the edge of a chunk wraps around the chunk.
            if(place_block)
            {
                if(blockx < 0)
                {
                    closest_chunk = world_find_chunk(world, closest_chunk->x-1, closest_chunk->z);
                    blockx += CHUNK_SIZE;
                }
                else if(blockx >= CHUNK_SIZE)
                {
                    closest_chunk = world_find_chunk(world, closest_chunk->x+1, closest_chunk->z);
                    blockx -= CHUNK_SIZE;
                }
                else if(blockz < 0)
                {
                    closest_chunk = world_find_chunk(world, closest_chunk->x, closest_chunk->z-1);
                    blockz += CHUNK_SIZE;
                }
                else if(blockz >= CHUNK_SIZE)
                {
                    closest_chunk = world_find_chunk(world, closest_chunk->x, closest_chunk->z+1);
                    blockz -= CHUNK_SIZE;

                }
                else if(blocky < 0 || blocky >= CHUNK_HEIGHT)
                {
                    goto dontmodify;
                }
                if(!closest_chunk)
                {
                    goto dontmodify;
                }
            }

            world_update_block(world, *closest_chunk, atlas, blockx, blocky, blockz, place_block ? BLOCK_TYPE_STONE : BLOCK_TYPE_AIR);
        }
dontmodify:;
    }
}