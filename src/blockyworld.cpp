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

#if 1
int main(int argc, char* argv[])
#else
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
#endif
{
    // Initialization
    const int screenWidth = 1770;
    const int screenHeight = 960;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
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
        player_update(&(world.player));

        Vector2 mouse_delta = GetMouseDelta();
        float sensitivity = 1/8.0f;
        SetMousePosition(GetScreenWidth()/2, GetScreenHeight()/2);

        // print_vec3(player.camera.position);

        // Draw
        BeginDrawing();

            ClearBackground(SKYBLUE);

            BeginMode3D(world.player.camera);

                rlBegin(RL_TRIANGLES);

                    rlColor3f( 1, 0, 0 ); // red
                    rlVertex3f( 0, 0.9, 1 );
                    rlColor3f( 0, 1, 0 ); // green
                    rlVertex3f( 0.8, -0.8, 1 );
                    rlColor3f( 0, 0, 1 ); // blue
                    rlVertex3f( -0.8, -0.8, 1 );

                rlEnd(); 

                world_update(&world, atlas);

            EndMode3D();

        EndDrawing();
    }

    // De-Initialization
    CloseWindow(); // Close window and OpenGL context

    return 0;
}