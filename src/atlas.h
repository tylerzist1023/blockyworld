#pragma once
#include "raylib.h"

// I don't know if I even used this...
static const int ATLAS_MAPPINGS[] = 
{
    0, // GRASS
};

struct Atlas
{
    Texture2D texture;
    int block_size;
};

Atlas atlas_create(const char* filename, int block_size=16);
Vector4 atlas_index_to_uv(Atlas atlas, int index);