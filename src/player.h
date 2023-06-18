#pragma once
#include "raylib.h"

struct Player
{
    Camera camera;
};

void player_init(Player *player);
void player_update(Player *player);