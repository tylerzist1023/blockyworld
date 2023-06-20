#pragma once
#include "atlas.h"
#include "containers.h"

enum BlockType
{
    BLOCK_TYPE_AIR = 0,
    BLOCK_TYPE_GRASS,
    BLOCK_TYPE_DIRT,
    BLOCK_TYPE_STONE,
};

enum BlockFace
{
    FRONT=0,
    RIGHT,
    BACK,
    LEFT,
    BOTTOM,
    TOP
};

// Information about the block type
struct BlockInfo
{
    int type;
    int cube_map[6];
    int animation_frames;
    bool is_cube;
};

// An instance of a block (will change once we get the world stuff set up)
struct Block
{
    BlockType type;
    int x,y,z;
};

#define CHUNK_HEIGHT 128
#define CHUNK_SIZE 16

struct ChunkData
{
    Block blocks[CHUNK_HEIGHT][CHUNK_SIZE][CHUNK_SIZE];
    Array<float> vertices = {0};
    Array<float> texcoords = {0};
    Array<float> normals = {0};

    Model finished_model = {0};
};

struct Chunk
{
    int x=0,z=0;
    ChunkData *data = 0;
};

Chunk chunk_generate(int x, int z);
void chunk_destroy(Chunk chunk);
void chunk_render(Atlas atlas, Chunk chunk);

struct World
{
    Array<Chunk> chunks;
    int max_chunks = 64;
    int chunk_counter = 0;

    Player player;
};

void world_update(World *world, Atlas atlas);