#pragma once
#include "raylib.h"

struct Player
{
    Camera camera;
};

struct World;
struct Atlas;

void player_init(Player *player);
void player_update(Player *player, World *world, Atlas atlas);