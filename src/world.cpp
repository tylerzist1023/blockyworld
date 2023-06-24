#include "world.h"
#include "atlas.h"
#include "noise.h"
#include "raylib.h"
#include <stdlib.h>
#include <string.h>

static const float CUBE_VERTICES[] = 
{
    // Front face
    0.0f, 0.0f, 1.0f, // Vertex 0
    1.0f, 0.0f, 1.0f, // Vertex 1
    1.0f, 1.0f, 1.0f, // Vertex 2

    1.0f, 1.0f, 1.0f, // Vertex 2
    0.0f, 1.0f, 1.0f, // Vertex 3
    0.0f, 0.0f, 1.0f, // Vertex 0

    // Right face
    1.0f, 0.0f, 1.0f, // Vertex 1
    1.0f, 0.0f, 0.0f, // Vertex 2
    1.0f, 1.0f, 0.0f, // Vertex 3

    1.0f, 1.0f, 0.0f, // Vertex 3
    1.0f, 1.0f, 1.0f, // Vertex 4
    1.0f, 0.0f, 1.0f, // Vertex 1

    // Back face
    1.0f, 0.0f, 0.0f, // Vertex 2
    0.0f, 0.0f, 0.0f, // Vertex 3
    0.0f, 1.0f, 0.0f, // Vertex 4

    0.0f, 1.0f, 0.0f, // Vertex 4
    1.0f, 1.0f, 0.0f, // Vertex 5
    1.0f, 0.0f, 0.0f, // Vertex 2

    // Left face
    0.0f, 0.0f, 0.0f, // Vertex 3
    0.0f, 0.0f, 1.0f, // Vertex 4
    0.0f, 1.0f, 1.0f, // Vertex 5

    0.0f, 1.0f, 1.0f, // Vertex 5
    0.0f, 1.0f, 0.0f, // Vertex 6
    0.0f, 0.0f, 0.0f, // Vertex 3

    // Bottom face
    0.0f, 0.0f, 0.0f, // Vertex 4
    1.0f, 0.0f, 0.0f, // Vertex 5
    1.0f, 0.0f, 1.0f, // Vertex 6

    1.0f, 0.0f, 1.0f, // Vertex 6
    0.0f, 0.0f, 1.0f, // Vertex 7
    0.0f, 0.0f, 0.0f, // Vertex 4

    // Top face
    0.0f, 1.0f, 1.0f, // Vertex 5
    1.0f, 1.0f, 1.0f, // Vertex 6
    1.0f, 1.0f, 0.0f, // Vertex 7

    1.0f, 1.0f, 0.0f, // Vertex 7
    0.0f, 1.0f, 0.0f, // Vertex 4
    0.0f, 1.0f, 1.0f, // Vertex 5
};

static const float CUBE_NORMALS[] = 
{
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,

    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,

    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,

    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,

    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,

    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
};

static inline void fuse_block_to_chunk(ChunkData *chunk_data, int x, int y, int z, Atlas atlas)
{
    float texcoords[70];
    for(int i = 0; i < 6; i++)
    {
        Vector4 uv_coords = atlas_index_to_uv(atlas, BLOCK_INFO[chunk_data->blocks[y][x][z].type].cube_map[i]);

        texcoords[i*12] = uv_coords.x;
        texcoords[i*12+1] = uv_coords.w;

        texcoords[i*12+2] = uv_coords.z;
        texcoords[i*12+3] = uv_coords.w;

        texcoords[i*12+4] = uv_coords.z;
        texcoords[i*12+5] = uv_coords.y;

        texcoords[i*12+6] = uv_coords.z;
        texcoords[i*12+7] = uv_coords.y;

        texcoords[i*12+8] = uv_coords.x;
        texcoords[i*12+9] = uv_coords.y;

        texcoords[i*12+10] = uv_coords.x;
        texcoords[i*12+11] = uv_coords.w;
    }

    // i: up, j: forward, k: left
    // [y][x][z]

    if(z < CHUNK_SIZE-1 && !BLOCK_INFO[chunk_data->blocks[y][x][z+1].type].is_cube)
    {
        for(int i = 0; i < 6; i++)
        {
            chunk_data->vertices.append(CUBE_VERTICES[18*FRONT+i*3]+x);
            chunk_data->vertices.append(CUBE_VERTICES[18*FRONT+i*3+1]+y);
            chunk_data->vertices.append(CUBE_VERTICES[18*FRONT+i*3+2]+z);
            chunk_data->texcoords.append(texcoords[12*FRONT+i*2]);
            chunk_data->texcoords.append(texcoords[12*FRONT+i*2+1]);
            chunk_data->normals.append(CUBE_NORMALS[18*FRONT+i*3]);
            chunk_data->normals.append(CUBE_NORMALS[18*FRONT+i*3+1]);
            chunk_data->normals.append(CUBE_NORMALS[18*FRONT+i*3+2]);
        }
    }
    if(x < CHUNK_SIZE-1 && !BLOCK_INFO[chunk_data->blocks[y][x+1][z].type].is_cube)
    {
        for(int i = 0; i < 6; i++)
        {
            chunk_data->vertices.append(CUBE_VERTICES[18*RIGHT+i*3]+x);
            chunk_data->vertices.append(CUBE_VERTICES[18*RIGHT+i*3+1]+y);
            chunk_data->vertices.append(CUBE_VERTICES[18*RIGHT+i*3+2]+z);
            chunk_data->texcoords.append(texcoords[12*RIGHT+i*2]);
            chunk_data->texcoords.append(texcoords[12*RIGHT+i*2+1]);
            chunk_data->normals.append(CUBE_NORMALS[18*RIGHT+i*3]);
            chunk_data->normals.append(CUBE_NORMALS[18*RIGHT+i*3+1]);
            chunk_data->normals.append(CUBE_NORMALS[18*RIGHT+i*3+2]);
        }
    }
    if(z > 0 && !BLOCK_INFO[chunk_data->blocks[y][x][z-1].type].is_cube)
    {
        for(int i = 0; i < 6; i++)
        {
            chunk_data->vertices.append(CUBE_VERTICES[18*BACK+i*3]+x);
            chunk_data->vertices.append(CUBE_VERTICES[18*BACK+i*3+1]+y);
            chunk_data->vertices.append(CUBE_VERTICES[18*BACK+i*3+2]+z);
            chunk_data->texcoords.append(texcoords[12*BACK+i*2]);
            chunk_data->texcoords.append(texcoords[12*BACK+i*2+1]);
            chunk_data->normals.append(CUBE_NORMALS[18*BACK+i*3]);
            chunk_data->normals.append(CUBE_NORMALS[18*BACK+i*3+1]);
            chunk_data->normals.append(CUBE_NORMALS[18*BACK+i*3+2]);
        }
    }
    if(x > 0 && !BLOCK_INFO[chunk_data->blocks[y][x-1][z].type].is_cube)
    {
        for(int i = 0; i < 6; i++)
        {
            chunk_data->vertices.append(CUBE_VERTICES[18*LEFT+i*3]+x);
            chunk_data->vertices.append(CUBE_VERTICES[18*LEFT+i*3+1]+y);
            chunk_data->vertices.append(CUBE_VERTICES[18*LEFT+i*3+2]+z);
            chunk_data->texcoords.append(texcoords[12*LEFT+i*2]);
            chunk_data->texcoords.append(texcoords[12*LEFT+i*2+1]);
            chunk_data->normals.append(CUBE_NORMALS[18*LEFT+i*3]);
            chunk_data->normals.append(CUBE_NORMALS[18*LEFT+i*3+1]);
            chunk_data->normals.append(CUBE_NORMALS[18*LEFT+i*3+2]);
        }
    }
    if(y == 0 || !BLOCK_INFO[chunk_data->blocks[y-1][x][z].type].is_cube)
    {
        for(int i = 0; i < 6; i++)
        {
            chunk_data->vertices.append(CUBE_VERTICES[18*BOTTOM+i*3]+x);
            chunk_data->vertices.append(CUBE_VERTICES[18*BOTTOM+i*3+1]+y);
            chunk_data->vertices.append(CUBE_VERTICES[18*BOTTOM+i*3+2]+z);
            chunk_data->texcoords.append(texcoords[12*BOTTOM+i*2]);
            chunk_data->texcoords.append(texcoords[12*BOTTOM+i*2+1]);
            chunk_data->normals.append(CUBE_NORMALS[18*BOTTOM+i*3]);
            chunk_data->normals.append(CUBE_NORMALS[18*BOTTOM+i*3+1]);
            chunk_data->normals.append(CUBE_NORMALS[18*BOTTOM+i*3+2]);
        }
    }
    if(y == CHUNK_HEIGHT-1 || !BLOCK_INFO[chunk_data->blocks[y+1][x][z].type].is_cube)
    {
        for(int i = 0; i < 6; i++)
        {
            chunk_data->vertices.append(CUBE_VERTICES[18*TOP+i*3]+x);
            chunk_data->vertices.append(CUBE_VERTICES[18*TOP+i*3+1]+y);
            chunk_data->vertices.append(CUBE_VERTICES[18*TOP+i*3+2]+z);
            chunk_data->texcoords.append(texcoords[12*TOP+i*2]);
            chunk_data->texcoords.append(texcoords[12*TOP+i*2+1]);
            chunk_data->normals.append(CUBE_NORMALS[18*TOP+i*3]);
            chunk_data->normals.append(CUBE_NORMALS[18*TOP+i*3+1]);
            chunk_data->normals.append(CUBE_NORMALS[18*TOP+i*3+2]);
        }
    }
}

static inline void generate_chunk_data_model(ChunkData *chunk_data)
{
    Mesh mesh = {0};

    mesh.vertices = (float *)malloc(chunk_data->vertices.size*sizeof(float));
    memcpy(mesh.vertices, chunk_data->vertices.data, chunk_data->vertices.size*sizeof(float));

    mesh.texcoords = (float *)malloc(chunk_data->texcoords.size*sizeof(float));
    memcpy(mesh.texcoords, chunk_data->texcoords.data, chunk_data->texcoords.size*sizeof(float));

    mesh.normals = (float *)malloc(chunk_data->normals.size*sizeof(float));
    memcpy(mesh.normals, chunk_data->normals.data, chunk_data->normals.size*sizeof(float));

    mesh.indices = 0;// (unsigned short*)malloc(chunk_data->indices.size * sizeof(unsigned short));
    //memcpy(mesh.indices, chunk_data->indices.data, chunk_data->indices.size*sizeof(unsigned short));

    mesh.vertexCount = chunk_data->vertices.size/3;
    mesh.triangleCount = mesh.vertexCount/3;

    if(chunk_data->finished_model.meshCount != 0)
    {
        UnloadModel(chunk_data->finished_model);
    }

    UploadMesh(&mesh, true);
    Model model = LoadModelFromMesh(mesh);
    chunk_data->finished_model = model;
    //UnloadMesh(mesh);
}

static inline void add_face_to_chunk(ChunkData *chunk_data, Atlas atlas, BlockFace face, int x, int y, int z)
{  
    float texcoords[12];

    Vector4 uv_coords = atlas_index_to_uv(atlas, BLOCK_INFO[chunk_data->blocks[y][x][z].type].cube_map[face]);

    texcoords[0] = uv_coords.x;
    texcoords[1] = uv_coords.w;
    texcoords[2] = uv_coords.z;
    texcoords[3] = uv_coords.w;
    texcoords[4] = uv_coords.z;
    texcoords[5] = uv_coords.y;
    texcoords[6] = uv_coords.z;
    texcoords[7] = uv_coords.y;
    texcoords[8] = uv_coords.x;
    texcoords[9] = uv_coords.y;
    texcoords[10] = uv_coords.x;
    texcoords[11] = uv_coords.w;

    for(int i = 0; i < 6; i++)
    {
        chunk_data->vertices.append(CUBE_VERTICES[18*face+i*3]+x);
        chunk_data->vertices.append(CUBE_VERTICES[18*face+i*3+1]+y);
        chunk_data->vertices.append(CUBE_VERTICES[18*face+i*3+2]+z);
    }
    for(int i = 0; i < 12; i++)
    {
        chunk_data->texcoords.append(texcoords[i]);
    }
    for(int i = 0; i < 36; i++)
    {
        chunk_data->normals.append(CUBE_NORMALS[i]);
    }
}

void chunk_update_model(ChunkData *chunk_data, Atlas atlas)
{
    chunk_data->vertices.clear();
    chunk_data->normals.clear();
    chunk_data->texcoords.clear();

    for(int i = 0; i < CHUNK_HEIGHT; i++)
    {
        for(int j = 0; j < CHUNK_SIZE; j++)
        {
            for(int k = 0; k < CHUNK_SIZE; k++)
            {
                if(BLOCK_INFO[chunk_data->blocks[i][j][k].type].is_cube)
                {
                    fuse_block_to_chunk(chunk_data, j, i, k, atlas);
                }
            }
        }
    }
    generate_chunk_data_model(chunk_data);
}

Chunk chunk_generate(int x, int z, Atlas atlas, ChunkEditHistory *history)
{
    srand(x^z);

    Chunk chunk = {0};
    chunk.data = (ChunkData *)malloc(sizeof(*(chunk.data)));
    memset(chunk.data, 0, sizeof(*(chunk.data)));

    for(int j = 0; j < CHUNK_SIZE; j++)
    {
        for(int k = 0; k < CHUNK_SIZE; k++)
        {
            float perlin_height = perlin2d((float)(x-0.5)*CHUNK_SIZE+j, (float)(z-0.5)*CHUNK_SIZE+k, .01, 3);
            perlin_height*=32.0;
            for(int i = 0; i < CHUNK_HEIGHT; i++)
            {
                if(i < perlin_height/2)
                {
                    chunk.data->blocks[i][j][k].type = BLOCK_TYPE_STONE;
                }
                else if(i < perlin_height-1)
                {
                    chunk.data->blocks[i][j][k].type = BLOCK_TYPE_DIRT;
                }
                else if(i < perlin_height)
                {
                    chunk.data->blocks[i][j][k].type = BLOCK_TYPE_GRASS;
                }
                else
                {
                    break;
                    // chunk.data->blocks[i][j][k].type = BLOCK_TYPE_AIR;
                }
                chunk.data->blocks[i][j][k].x = j;
                chunk.data->blocks[i][j][k].y = i;
                chunk.data->blocks[i][j][k].z = k;
            }
        }
    }

    if(history)
    {
        for(int i = 0; i < history->edits.size; i++)
        {
            int bx = history->edits[i].x;
            int by = history->edits[i].y;
            int bz = history->edits[i].z;

            chunk.data->blocks[by][bx][bz].type = history->edits[i].type;
        }
    }

    chunk_update_model(chunk.data, atlas);

    chunk.x = x;
    chunk.z = z;

    return chunk;
}


void chunk_destroy(Chunk chunk)
{
    chunk.data->vertices.destroy();
    chunk.data->texcoords.destroy();
    chunk.data->normals.destroy();
    UnloadModel(chunk.data->finished_model);
    free(chunk.data);
}

void chunk_render(Atlas atlas, Chunk chunk)
{
    chunk.data->finished_model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = atlas.texture;
    DrawModel(chunk.data->finished_model, {(float)chunk.x*CHUNK_SIZE, 0.f, (float)chunk.z*CHUNK_SIZE}, 1.f, WHITE);
}

Chunk *world_find_chunk(World *world, int x, int z)
{
    for(int i = 0; i < world->chunks.size; i++)
    {
        if(world->chunks[i].x == x && world->chunks[i].z == z)
        {
            return &(world->chunks[i]);
        }
    }
    return 0;
}

ChunkEditHistory *world_find_edit_history(World *world, int x, int z)
{
    for(int i = 0; i < world->histories.size; i++)
    {
        if(world->histories[i].x == x && world->histories[i].z == z)
        {
            return &(world->histories[i]);
        }
    }
    return 0;
}

static inline void add_neighbor_edge_faces(Chunk chunk, Chunk neighbor, BlockFace face, Atlas atlas)
{
    int chunk_edge=0, neighbor_edge=0;
    if(face == FRONT || face == RIGHT)
    {
        chunk_edge = 0;
        neighbor_edge = CHUNK_SIZE-1;
    }
    else if(face == BACK || face == LEFT)
    {
        chunk_edge = CHUNK_SIZE-1;
        neighbor_edge = 0;
    }

    if(face == FRONT || face == BACK)
    {
        for(int i = 0; i < CHUNK_HEIGHT; i++)
        {
            for(int j = 0; j < CHUNK_SIZE; j++)
            {
                if(!BLOCK_INFO[chunk.data->blocks[i][j][chunk_edge].type].is_cube && BLOCK_INFO[neighbor.data->blocks[i][j][neighbor_edge].type].is_cube)
                {
                    add_face_to_chunk(neighbor.data, atlas, face, j, i, neighbor_edge);
                }
            }
        }
    }
    else if(face == LEFT || face == RIGHT)
    {
        for(int i = 0; i < CHUNK_HEIGHT; i++)
        {
            for(int j = 0; j < CHUNK_SIZE; j++)
            {
                if(!BLOCK_INFO[chunk.data->blocks[i][chunk_edge][j].type].is_cube && BLOCK_INFO[neighbor.data->blocks[i][neighbor_edge][j].type].is_cube)
                {
                    add_face_to_chunk(neighbor.data, atlas, face, neighbor_edge, i, j);
                }
            }
        }
    }
}

void world_update_neighbors(World *world, Chunk chunk, Atlas atlas, bool updating_neighbors)
{
    Chunk *neighbors[4] =
    {
        world_find_chunk(world, chunk.x, chunk.z-1),
        world_find_chunk(world, chunk.x, chunk.z+1),
        world_find_chunk(world, chunk.x+1, chunk.z),
        world_find_chunk(world, chunk.x-1, chunk.z),
    };
    BlockFace faces[4] =
    {
        FRONT,
        BACK,
        LEFT,
        RIGHT,
    };

    for(int i = 0; i < 4; i++)
    {
        if(neighbors[i] == 0) continue;
        add_neighbor_edge_faces(chunk, *(neighbors[i]), faces[i], atlas);
        generate_chunk_data_model(neighbors[i]->data);
        if(!updating_neighbors) world_update_neighbors(world, *(neighbors[i]), atlas, true);
    }
}

void world_update_block(World *world, Chunk chunk, Atlas atlas, int x, int y, int z, BlockType type)
{
    chunk.data->blocks[y][x][z].type = type;
    chunk_update_model(chunk.data, atlas);
    world_update_neighbors(world, chunk, atlas);

    ChunkEditHistory *history = world_find_edit_history(world, chunk.x, chunk.z);
    if(!history)
    {
        world->histories.append({chunk.x, chunk.z, {0}});
        world->histories[world->histories.size-1].edits.append({x,y,z,type});
    }
    else
    {
        history->edits.append({x,y,z,type});
    }
}

static inline void get_next_chunk(World *world, int x, int z, Atlas atlas, int depth=0)
{
    if(world_find_chunk(world, x, z))
    {
        if(depth*depth*depth*depth > world->max_chunks)
        {
            return;
        }

        get_next_chunk(world, x+1, z, atlas, depth+1);
        get_next_chunk(world, x-1, z, atlas, depth+1);
        get_next_chunk(world, x, z+1, atlas, depth+1);
        get_next_chunk(world, x, z-1, atlas, depth+1);
    }
    else
    {
        ChunkEditHistory *history = world_find_edit_history(world, x, z);
        Chunk chunk = chunk_generate(x, z, atlas, history);

        if(world->chunks.size < world->max_chunks)
        {
            world->chunks.append(chunk);
        }
        else
        {
            chunk_destroy(world->chunks[world->chunk_counter%world->max_chunks]);
            world->chunks[world->chunk_counter%world->max_chunks] = chunk;
        }
        world->chunk_counter++;
        world_update_neighbors(world, chunk, atlas);
    }
}

void world_update(World *world, Atlas atlas)
{
    get_next_chunk(world, world->player.camera.position.x/CHUNK_SIZE,  world->player.camera.position.z/CHUNK_SIZE, atlas);

    for(int i = 0; i < world->chunks.size; i++)
    {
        chunk_render(atlas, world->chunks[i]);
    }
}