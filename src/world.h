#pragma once
#include "atlas.h"
#include "containers.h"

enum BlockType
{
    BLOCK_TYPE_GRASS = 0,
    BLOCK_TYPE_AIR = 1,
};

// Information about the block type
struct BlockInfo
{
    int type;
    int texture_id;
    int animation_frames;
    bool is_cube;
};

// An instance of a block (will change once we get the world stuff set up)
struct Block
{
    BlockType type;
    int x,y,z;
};

#define CHUNK_HEIGHT 256
#define CHUNK_SIZE 16

struct ChunkData
{
    Block blocks[CHUNK_HEIGHT][CHUNK_SIZE][CHUNK_SIZE];
    Array<float> vertices;
    Array<float> texcoords;
    Array<float> normals;

    Model finished_model = {0};
};

struct Chunk
{
    int x,z;
    ChunkData *data;
};

struct World
{
    
};

Chunk chunk_generate(int x, int z);
void chunk_destroy(Chunk chunk);
void chunk_render(Atlas atlas, Chunk chunk);