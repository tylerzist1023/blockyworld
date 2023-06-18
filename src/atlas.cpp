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

Vector4 atlas_index_to_uv(Atlas atlas, int index)
{
    int x,y,z,w;
    x = (atlas.block_size*index) % atlas.texture.width;
    y = ((atlas.block_size*index) / atlas.texture.width) * atlas.block_size;
    z = x + atlas.block_size;
    w = y + atlas.block_size;
    Vector4 coords;
    coords.x = (float)x/atlas.texture.width;
    coords.y = (float)y/atlas.texture.height;
    coords.z = (float)z/atlas.texture.width;
    coords.w = (float)w/atlas.texture.height;
    return coords;
}