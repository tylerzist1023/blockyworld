#include "atlas.h "
#include "raylib.h "
#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"
#include "rcamera.h"
#include "player.h"
#include "world.h"
#include <stdio.h>

#if defined(__EMSCRIPTEN__)
    #define GLFW_INCLUDE_ES2
#endif

#pragma comment(lib, "raylib")
#pragma comment(lib, "opengl32")
#pragma comment(lib, "Gdi32")
#pragma comment(lib, "WinMM")
#pragma comment(lib, "User32")
#pragma comment(lib, "shell32")

void print_vec3(Vector3 v)
{
    printf("{%f, %f, %f}\n", v.x, v.y, v.z);
}

void CustomLog(int msgType, const char *text, va_list args)
{
    return;
}

#if 1
int main(int argc, char* argv[])
#else
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
#endif
{
    SetTraceLogLevel(0);
    SetTraceLogCallback(CustomLog);

    // Initialization
    const int screenWidth = 960;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Very Original Game Concept");
    SetWindowState(FLAG_WINDOW_RESIZABLE);

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second

    DisableCursor();

    Atlas atlas = atlas_create("res/texture.png");

    World world;
    player_init(&(world.player));

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        

        // print_vec3(player.camera.position);

        // Draw
        BeginDrawing();

            ClearBackground(SKYBLUE);

            BeginMode3D(world.player.camera);

                player_update(&(world.player), &world, atlas);

                Vector2 mouse_delta = GetMouseDelta();
                float sensitivity = 1/8.0f;
                SetMousePosition(GetScreenWidth()/2, GetScreenHeight()/2);

                world_update(&world, atlas);

            EndMode3D();

        EndDrawing();
    }

    // De-Initialization
    CloseWindow(); // Close window and OpenGL context

    return 0;
}