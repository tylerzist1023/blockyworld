#include "atlas.h"
#include "raylib.h"

Atlas atlas_create(const char *filename, int block_size)
{
    Atlas atlas = {0};

    Image atlas_image = LoadImage(filename);
    atlas.texture = LoadTextureFromImage(atlas_image);
    atlas.block_size = block_size;
    UnloadImage(atlas_image);

    return atlas;
}

AtlasCoords atlas_index_to_coords(Atlas atlas, int index)
{
    return {(atlas.block_size*index) % atlas.texture.width, (atlas.block_size*index) / atlas.texture.width};
}