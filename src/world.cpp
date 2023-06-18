#include "world.h"
#include "raylib.h"
#include <stdlib.h>
#include <string.h>

static Model block_model = {0};

static const BlockInfo BLOCK_INFO[] = 
{
    {BLOCK_TYPE_GRASS, 0, 1, true},
    {BLOCK_TYPE_AIR, 0, 1, false},
};


void fuse_block_to_chunk(ChunkData *chunk_data, int x, int y, int z)
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

    float texcoords[] =
    {
        // Front face
        0.0f, 0.0f,   // Vertex 0
        1.0f, 0.0f,   // Vertex 1
        1.0f, 1.0f,   // Vertex 2

        1.0f, 1.0f,   // Vertex 2
        0.0f, 1.0f,   // Vertex 3
        0.0f, 0.0f,   // Vertex 0

        // Right face
        0.0f, 0.0f,   // Vertex 1
        1.0f, 0.0f,   // Vertex 2
        1.0f, 1.0f,   // Vertex 3

        1.0f, 1.0f,   // Vertex 3
        0.0f, 1.0f,   // Vertex 4
        0.0f, 0.0f,   // Vertex 1

        // Back face
        0.0f, 0.0f,   // Vertex 2
        1.0f, 0.0f,   // Vertex 3
        1.0f, 1.0f,   // Vertex 4

        1.0f, 1.0f,   // Vertex 4
        0.0f, 1.0f,   // Vertex 5
        0.0f, 0.0f,   // Vertex 2

        // Left face
        0.0f, 0.0f,   // Vertex 3
        1.0f, 0.0f,   // Vertex 4
        1.0f, 1.0f,   // Vertex 5

        1.0f, 1.0f,   // Vertex 5
        0.0f, 1.0f,   // Vertex 6
        0.0f, 0.0f,   // Vertex 3

        // Bottom face
        0.0f, 0.0f,   // Vertex 4
        1.0f, 0.0f,   // Vertex 5
        1.0f, 1.0f,   // Vertex 6

        1.0f, 1.0f,   // Vertex 6
        0.0f, 1.0f,   // Vertex 7
        0.0f, 0.0f,   // Vertex 4

        // Top face
        0.0f, 0.0f,   // Vertex 5
        1.0f, 0.0f,   // Vertex 6
        1.0f, 1.0f,   // Vertex 7

        1.0f, 1.0f,   // Vertex 7
        0.0f, 1.0f,   // Vertex 4
        0.0f, 0.0f,   // Vertex 5
    };

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

    enum Faces
    {
        FRONT=0,
        RIGHT,
        BACK,
        LEFT,
        BOTTOM,
        TOP
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

    // for(int i = 0; i < 36; i++)
    // {
    //     vertices[3*i]   += x;
    //     vertices[3*i+1] += y;
    //     vertices[3*i+2] += z;

    //     chunk_data->vertices.append(vertices[3*i]);
    //     chunk_data->vertices.append(vertices[3*i+1]);
    //     chunk_data->vertices.append(vertices[3*i+2]);
    // }

    // for(int i = 0; i < 36; i++)
    // {
    //     chunk_data->texcoords.append(texcoords[2*i]);
    //     chunk_data->texcoords.append(texcoords[2*i+1]);
    // }

    // for(int i = 0; i < 36; i++)
    // {
    //     chunk_data->normals.append(normals[3*i]);
    //     chunk_data->normals.append(normals[3*i+1]);
    //     chunk_data->normals.append(normals[3*i+2]);
    // }

    // int k = chunk_data->indices.size/6;
    // unsigned short indices[36];
    // for (int i = 0; i < 36; i += 6)
    // {
    //     indices[i] = 4*k;
    //     indices[i + 1] = 4*k + 1;
    //     indices[i + 2] = 4*k + 2;
    //     indices[i + 3] = 4*k;
    //     indices[i + 4] = 4*k + 2;
    //     indices[i + 5] = 4*k + 3;

    //     k++;
    // }

    // for (int i = 0; i < 36; i++)
    // {
    //     chunk_data->indices.append(indices[i]);
    // }
}

void make_model_from_chunk_data(ChunkData *chunk_data)
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

    UploadMesh(&mesh, false);
    Model model = LoadModelFromMesh(mesh);
    chunk_data->finished_model = model;
    //UnloadMesh(mesh);
}

Chunk chunk_generate(int x, int z)
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
                if(i < 32)
                {
                    chunk.data->blocks[i][j][k].type = BLOCK_TYPE_GRASS;
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
                    fuse_block_to_chunk(chunk.data, j, i, k);
                }
            }
        }
    }
    make_model_from_chunk_data(chunk.data);

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

void world_update(World *world, Atlas atlas)
{
    world_get_next_chunk(world, world->player.camera.position.x/CHUNK_SIZE,  world->player.camera.position.z/CHUNK_SIZE);

    for(int i = 0; i < world->chunks.size; i++)
    {
        chunk_render(atlas, world->chunks[i]);
    }
}

bool chunk_exists_in_world(World *world, int x, int z)
{
    for(int i = 0; i < world->chunks.size; i++)
    {
        if(world->chunks[i].x == x && world->chunks[i].z == z)
        {
            return true;
        }
    }
    return false;
}

void world_get_next_chunk(World *world, int x, int z, int depth)
{
    if(chunk_exists_in_world(world, x, z))
    {
        if(depth*depth*depth*depth > world->max_chunks)
        {
            return;
        }

        world_get_next_chunk(world, x+1, z, depth+1);
        world_get_next_chunk(world, x-1, z, depth+1);
        world_get_next_chunk(world, x, z+1, depth+1);
        world_get_next_chunk(world, x, z-1, depth+1);
    }
    else
    {
        Chunk chunk = chunk_generate(x, z);

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