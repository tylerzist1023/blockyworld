#pragma once
#include "raylib.h"

static const int ATLAS_MAPPINGS[] = 
{
    0, // GRASS
};

struct Atlas
{
    Texture2D texture;
    int block_size;
};

struct AtlasCoords
{
    int x,y;
};

Atlas atlas_create(const char* filename, int block_size=16);
AtlasCoords atlas_index_to_coords(Atlas atlas, int index);