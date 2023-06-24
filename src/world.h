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

static const BlockInfo BLOCK_INFO[] = 
{
    {BLOCK_TYPE_AIR, {0,0,0,0,0,0}, 1, false},
    {BLOCK_TYPE_GRASS, {2,2,2,2,1,0}, 1, true},
    {BLOCK_TYPE_DIRT, {1,1,1,1,1,1}, 1, true},
    {BLOCK_TYPE_STONE, {3,3,3,3,3,3}, 1, true},
};

// An instance of a block (will change once we get the world stuff set up)
struct Block
{
    int x,y,z;
    BlockType type;
};

#define CHUNK_HEIGHT 128
#define CHUNK_SIZE 16

struct ChunkEditHistory
{
    int x,z;
    Array<Block> edits;
};

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

void chunk_update_model(ChunkData *chunk_data, Atlas atlas);
Chunk chunk_generate(int x, int z, Atlas atlas, ChunkEditHistory *history);
void chunk_destroy(Chunk chunk);
void chunk_render(Atlas atlas, Chunk chunk);

struct World
{
    // TODO: Use some kind of hash table. And make it so we don't regenerate block data every time a chunk gets reloaded in memory.
    Array<Chunk> chunks;
    int max_chunks = 32;
    int chunk_counter = 0;

    Array<ChunkEditHistory> histories;

    Player player;
};

void world_update(World *world, Atlas atlas);
void world_update_neighbors(World *world, Chunk chunk, Atlas atlas, bool updating_neighbors=false);
Chunk *world_find_chunk(World *world, int x, int z);
ChunkEditHistory *world_find_edit_history(World *world, int x, int z);
void world_update_block(World *world, Chunk chunk, Atlas atlas, int x, int y, int z, BlockType type);