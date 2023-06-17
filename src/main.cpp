#include "raylib.h"
#include "rlgl.h"

#if defined(__EMSCRIPTEN__)
    #define GLFW_INCLUDE_ES2
#endif

#pragma comment(lib, "raylib")
#pragma comment(lib, "opengl32")
#pragma comment(lib, "Gdi32")
#pragma comment(lib, "WinMM")
#pragma comment(lib, "User32")
#pragma comment(lib, "shell32")

#if 1
int main(int argc, char* argv[])
#else
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
#endif
{
    // Initialization
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    SetWindowState(FLAG_WINDOW_RESIZABLE);

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second

    Camera camera = {0};
    camera.position = {0.f, 0.f, -1.f};
    camera.target = {0.f, 0.f, 1.f};
    camera.up = {0.f, 1.f, 0.f};
    camera.fovy = 60.f;
    camera.projection = CAMERA_PERSPECTIVE;

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //UpdateCamera(&camera, CAMERA_ORBITAL);

        // Draw
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                rlBegin(RL_TRIANGLES);

                    rlColor3f( 1, 0, 0 ); // red
                    rlVertex3f( 0, 0.9, 1 );
                    rlColor3f( 0, 1, 0 ); // green
                    rlVertex3f( 0.8, -0.8, 1 );
                    rlColor3f( 0, 0, 1 ); // blue
                    rlVertex3f( -0.8, -0.8, 1 );

                rlEnd(); 

            EndMode3D();

        EndDrawing();
    }

    // De-Initialization
    CloseWindow(); // Close window and OpenGL context

    return 0;
}