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
    float width = 1.f, height = 1.f, length = 1.f;

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

    for(int i = 0; i < 36; i++)
    {
        vertices[3*i]   += x;
        vertices[3*i+1] += y;
        vertices[3*i+2] += z;

        chunk_data->vertices.append(vertices[3*i]);
        chunk_data->vertices.append(vertices[3*i+1]);
        chunk_data->vertices.append(vertices[3*i+2]);
    }

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

    for(int i = 0; i < 36; i++)
    {
        chunk_data->texcoords.append(texcoords[2*i]);
        chunk_data->texcoords.append(texcoords[2*i+1]);
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

    for(int i = 0; i < 36; i++)
    {
        chunk_data->normals.append(normals[3*i]);
        chunk_data->normals.append(normals[3*i+1]);
        chunk_data->normals.append(normals[3*i+2]);
    }

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
}

Chunk chunk_generate(int x, int z)
{
    Chunk chunk = {0};
    chunk.data = (ChunkData *)malloc(sizeof(*(chunk.data)));

    for(int i = 0; i < CHUNK_HEIGHT; i++)
    {
        for(int j = 0; j < CHUNK_SIZE; j++)
        {
            for(int k = 0; k < CHUNK_SIZE; k++)
            {
                if(i < 128)
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

                if(BLOCK_INFO[chunk.data->blocks[i][j][k].type].is_cube)
                {
                    fuse_block_to_chunk(chunk.data, j, i, k);
                }
            }
        }
    }

    make_model_from_chunk_data(chunk.data);

    return chunk;
}

void chunk_destroy(Chunk chunk)
{
    free(chunk.data);
}

void chunk_render(Atlas atlas, Chunk chunk)
{
    chunk.data->finished_model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = atlas.texture;
    DrawModel(chunk.data->finished_model, {(float)chunk.x, 0.f, (float)chunk.z}, 1.f, WHITE);
}