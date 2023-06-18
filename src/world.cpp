#include "world.h"
#include "atlas.h"
#include "raylib.h"
#include <stdlib.h>
#include <string.h>

static Model block_model = {0};

static const BlockInfo BLOCK_INFO[] = 
{
    {BLOCK_TYPE_GRASS, {2,2,2,2,1,0}, 1, true},
    {BLOCK_TYPE_DIRT, {1,1,1,1,1,1}, 1, true},
    {BLOCK_TYPE_STONE, {3,3,3,3,3,3}, 1, true},
    {BLOCK_TYPE_AIR, {0,0,0,0,0,0}, 1, false},
};


void fuse_block_to_chunk(ChunkData *chunk_data, int x, int y, int z, Atlas atlas)
{
    float vertices[] = 
    {
        // Front face
        -0.5f, -0.5f, 0.5f,   // Vertex 0
         0.5f, -0.5f, 0.5f,   // Vertex 1
         0.5f,  0.5f, 0.5f,   // Vertex 2

         0.5f,  0.5f, 0.5f,   // Vertex 2
        -0.5f,  0.5f, 0.5f,   // Vertex 3
        -0.5f, -0.5f, 0.5f,   // Vertex 0

        // Right face
         0.5f, -0.5f, 0.5f,   // Vertex 1
         0.5f, -0.5f, -0.5f,  // Vertex 2
         0.5f,  0.5f, -0.5f,  // Vertex 3

         0.5f,  0.5f, -0.5f,  // Vertex 3
         0.5f,  0.5f, 0.5f,   // Vertex 4
         0.5f, -0.5f, 0.5f,   // Vertex 1

        // Back face
         0.5f, -0.5f, -0.5f,  // Vertex 2
        -0.5f, -0.5f, -0.5f,  // Vertex 3
        -0.5f,  0.5f, -0.5f,  // Vertex 4

        -0.5f,  0.5f, -0.5f,  // Vertex 4
         0.5f,  0.5f, -0.5f,  // Vertex 5
         0.5f, -0.5f, -0.5f,  // Vertex 2

        // Left face
        -0.5f, -0.5f, -0.5f,  // Vertex 3
        -0.5f, -0.5f, 0.5f,   // Vertex 4
        -0.5f,  0.5f, 0.5f,   // Vertex 5

        -0.5f,  0.5f, 0.5f,   // Vertex 5
        -0.5f,  0.5f, -0.5f,  // Vertex 6
        -0.5f, -0.5f, -0.5f,  // Vertex 3

        // Bottom face
        -0.5f, -0.5f, -0.5f,  // Vertex 4
         0.5f, -0.5f, -0.5f,  // Vertex 5
         0.5f, -0.5f, 0.5f,   // Vertex 6

         0.5f, -0.5f, 0.5f,   // Vertex 6
        -0.5f, -0.5f, 0.5f,   // Vertex 7
        -0.5f, -0.5f, -0.5f,  // Vertex 4

        // Top face
        -0.5f,  0.5f, 0.5f,   // Vertex 5
         0.5f,  0.5f, 0.5f,   // Vertex 6
         0.5f,  0.5f, -0.5f,  // Vertex 7

         0.5f,  0.5f, -0.5f,  // Vertex 7
        -0.5f,  0.5f, -0.5f,  // Vertex 4
        -0.5f,  0.5f, 0.5f,   // Vertex 5
    };

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

    float normals[] = {
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,

        0.0f, 0.0f,-1.0f,
        0.0f, 0.0f,-1.0f,
        0.0f, 0.0f,-1.0f,
        0.0f, 0.0f,-1.0f,
        0.0f, 0.0f,-1.0f,
        0.0f, 0.0f,-1.0f,

        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,

        0.0f,-1.0f, 0.0f,
        0.0f,-1.0f, 0.0f,
        0.0f,-1.0f, 0.0f,
        0.0f,-1.0f, 0.0f,
        0.0f,-1.0f, 0.0f,
        0.0f,-1.0f, 0.0f,

        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,

        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
    };

    // i: up, j: forward, k: left
    // [y][z][x]

    if(z == CHUNK_SIZE-1 || !BLOCK_INFO[chunk_data->blocks[y][x][z+1].type].is_cube)
    {
        for(int i = 0; i < 6; i++)
        {
            chunk_data->vertices.append(vertices[18*FRONT+i*3]+x);
            chunk_data->vertices.append(vertices[18*FRONT+i*3+1]+y);
            chunk_data->vertices.append(vertices[18*FRONT+i*3+2]+z);
            chunk_data->texcoords.append(texcoords[12*FRONT+i*2]);
            chunk_data->texcoords.append(texcoords[12*FRONT+i*2+1]);
            chunk_data->normals.append(normals[18*FRONT+i*3]);
            chunk_data->normals.append(normals[18*FRONT+i*3+1]);
            chunk_data->normals.append(normals[18*FRONT+i*3+2]);
        }
    }
    if(x == CHUNK_SIZE-1 || !BLOCK_INFO[chunk_data->blocks[y][x+1][z].type].is_cube)
    {
        for(int i = 0; i < 6; i++)
        {
            chunk_data->vertices.append(vertices[18*RIGHT+i*3]+x);
            chunk_data->vertices.append(vertices[18*RIGHT+i*3+1]+y);
            chunk_data->vertices.append(vertices[18*RIGHT+i*3+2]+z);
            chunk_data->texcoords.append(texcoords[12*RIGHT+i*2]);
            chunk_data->texcoords.append(texcoords[12*RIGHT+i*2+1]);
            chunk_data->normals.append(normals[18*RIGHT+i*3]);
            chunk_data->normals.append(normals[18*RIGHT+i*3+1]);
            chunk_data->normals.append(normals[18*RIGHT+i*3+2]);
        }
    }
    if(z == 0 || !BLOCK_INFO[chunk_data->blocks[y][x][z-1].type].is_cube)
    {
        for(int i = 0; i < 6; i++)
        {
            chunk_data->vertices.append(vertices[18*BACK+i*3]+x);
            chunk_data->vertices.append(vertices[18*BACK+i*3+1]+y);
            chunk_data->vertices.append(vertices[18*BACK+i*3+2]+z);
            chunk_data->texcoords.append(texcoords[12*BACK+i*2]);
            chunk_data->texcoords.append(texcoords[12*BACK+i*2+1]);
            chunk_data->normals.append(normals[18*BACK+i*3]);
            chunk_data->normals.append(normals[18*BACK+i*3+1]);
            chunk_data->normals.append(normals[18*BACK+i*3+2]);
        }
    }
    if(x == 0 || !BLOCK_INFO[chunk_data->blocks[y][x-1][z].type].is_cube)
    {
        for(int i = 0; i < 6; i++)
        {
            chunk_data->vertices.append(vertices[18*LEFT+i*3]+x);
            chunk_data->vertices.append(vertices[18*LEFT+i*3+1]+y);
            chunk_data->vertices.append(vertices[18*LEFT+i*3+2]+z);
            chunk_data->texcoords.append(texcoords[12*LEFT+i*2]);
            chunk_data->texcoords.append(texcoords[12*LEFT+i*2+1]);
            chunk_data->normals.append(normals[18*LEFT+i*3]);
            chunk_data->normals.append(normals[18*LEFT+i*3+1]);
            chunk_data->normals.append(normals[18*LEFT+i*3+2]);
        }
    }
    if(y == 0 || !BLOCK_INFO[chunk_data->blocks[y-1][x][z].type].is_cube)
    {
        for(int i = 0; i < 6; i++)
        {
            chunk_data->vertices.append(vertices[18*BOTTOM+i*3]+x);
            chunk_data->vertices.append(vertices[18*BOTTOM+i*3+1]+y);
            chunk_data->vertices.append(vertices[18*BOTTOM+i*3+2]+z);
            chunk_data->texcoords.append(texcoords[12*BOTTOM+i*2]);
            chunk_data->texcoords.append(texcoords[12*BOTTOM+i*2+1]);
            chunk_data->normals.append(normals[18*BOTTOM+i*3]);
            chunk_data->normals.append(normals[18*BOTTOM+i*3+1]);
            chunk_data->normals.append(normals[18*BOTTOM+i*3+2]);
        }
    }
    if(y == CHUNK_HEIGHT-1 || !BLOCK_INFO[chunk_data->blocks[y+1][x][z].type].is_cube)
    {
        for(int i = 0; i < 6; i++)
        {
            chunk_data->vertices.append(vertices[18*TOP+i*3]+x);
            chunk_data->vertices.append(vertices[18*TOP+i*3+1]+y);
            chunk_data->vertices.append(vertices[18*TOP+i*3+2]+z);
            chunk_data->texcoords.append(texcoords[12*TOP+i*2]);
            chunk_data->texcoords.append(texcoords[12*TOP+i*2+1]);
            chunk_data->normals.append(normals[18*TOP+i*3]);
            chunk_data->normals.append(normals[18*TOP+i*3+1]);
            chunk_data->normals.append(normals[18*TOP+i*3+2]);
        }
    }
}

void generate_chunk_data_model(ChunkData *chunk_data)
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
    mesh.triangleCount = mesh.vertexCount/2;

    UploadMesh(&mesh, true);
    Model model = LoadModelFromMesh(mesh);
    chunk_data->finished_model = model;
    //UnloadMesh(mesh);
}

Chunk chunk_generate(int x, int z, Atlas atlas)
{
    Chunk chunk = {0};
    chunk.data = (ChunkData *)malloc(sizeof(*(chunk.data)));
    memset(chunk.data, 0, sizeof(*(chunk.data)));

    for(int i = 0; i < CHUNK_HEIGHT; i++)
    {
        for(int j = 0; j < CHUNK_SIZE; j++)
        {
            for(int k = 0; k < CHUNK_SIZE; k++)
            {
                if(i == 32 && rand()%6!=0)
                {
                    chunk.data->blocks[i][j][k].type = BLOCK_TYPE_GRASS;
                }
                else if(i < 20)
                {
                    chunk.data->blocks[i][j][k].type = BLOCK_TYPE_STONE;
                }
                else if(i < 32)
                {
                    chunk.data->blocks[i][j][k].type = BLOCK_TYPE_DIRT;
                }
                else
                {
                    chunk.data->blocks[i][j][k].type = BLOCK_TYPE_AIR;
                }
                chunk.data->blocks[i][j][k].x = j;
                chunk.data->blocks[i][j][k].y = i;
                chunk.data->blocks[i][j][k].z = k;
            }
        }
    }

    for(int i = 0; i < CHUNK_HEIGHT; i++)
    {
        for(int j = 0; j < CHUNK_SIZE; j++)
        {
            for(int k = 0; k < CHUNK_SIZE; k++)
            {
                if(BLOCK_INFO[chunk.data->blocks[i][j][k].type].is_cube)
                {
                    fuse_block_to_chunk(chunk.data, j, i, k, atlas);
                }
            }
        }
    }
    generate_chunk_data_model(chunk.data);

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
}

void chunk_render(Atlas atlas, Chunk chunk)
{
    chunk.data->finished_model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = atlas.texture;
    DrawModel(chunk.data->finished_model, {(float)chunk.x*CHUNK_SIZE, 0.f, (float)chunk.z*CHUNK_SIZE}, 1.f, WHITE);
}

Chunk* find_chunk_in_world(World *world, int x, int z)
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

void get_next_chunk(World *world, int x, int z, Atlas atlas, int depth=0)
{
    if(find_chunk_in_world(world, x, z))
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
        Chunk chunk = chunk_generate(x, z, atlas);

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
