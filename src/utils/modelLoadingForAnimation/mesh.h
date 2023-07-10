#pragma once

#include <string>
#include <vector>
#include "../common.h"
#include "../includes/stb_image.h"
#include "VertexUpdate.h"
#include <cuda.h>
#include <cuda_runtime_api.h>
#include <cuda_gl_interop.h>

using namespace std;
#pragma comment(lib, "cudart.lib")
#define gpuErrchk(ans)                        \
    {                                         \
        gpuAssert((ans), __FILE__, __LINE__); \
    }
inline void gpuAssert(cudaError_t code, const char *file, int line, bool abort = true)
{
    if (code != cudaSuccess)
    {
        fprintf(stderr, "GPUassert: %s %s %d\n", cudaGetErrorString(code), file, line);
        if (abort)
            exit(code);
    }
}

#define MAX_BONE_INFLUENCE 6

struct Vertex
{
    vmath::vec3 Position;
    vmath::vec3 Normal;
    vmath::vec2 TexCoords;
    vmath::vec3 Tangent;
    vmath::vec3 Bitangent;
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    float m_Weights[MAX_BONE_INFLUENCE];
};

struct Texture
{
    unsigned char *data;
    unsigned int id;
    string type;
    string path;
    int width;
    int height;
    int nrComponents;
};

struct Color
{
    vmath::vec3 ambiant_color;
    vmath::vec3 diffus_color;
    vmath::vec3 specular_color;
};

class Mesh
{
public:
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;
    Color color;
    unsigned int vao;
    unsigned int vaoParticles;
    vector<float> particleVertices;
    bool isDisintegration = false;

    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures, Color color)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;
        this->color = color;

        // Commented for multithreading as openGL functions need to be run in the main context [ thread]
        // initializeModel();
    }

    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures, Color color, vector<float> particleVertices)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;
        this->color = color;

        this->particleVertices = particleVertices;
        this->isDisintegration = true;
        // Commented for multithreading as openGL functions need to be run in the main context [ thread]
        // initializeModel();
    }

    void Render(GLuint shaderProgramObject)
    {

        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr = 1;
        unsigned int heightNr = 1;
        for (unsigned int i = 0; i < textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);

            string number;
            string name = textures[i].type;
            if (name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if (name == "texture_specular")
                number = std::to_string(specularNr++);
            else if (name == "texture_normal")
                number = std::to_string(normalNr++);
            else if (name == "texture_height")
                number = std::to_string(heightNr++);

            glUniform1i(glGetUniformLocation(shaderProgramObject, (name + number).c_str()), i);

            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }

        glUniform3fv(glGetUniformLocation(shaderProgramObject, "u_ka"), 1, color.ambiant_color);
        glUniform3fv(glGetUniformLocation(shaderProgramObject, "u_kd"), 1, color.diffus_color);
        glUniform3fv(glGetUniformLocation(shaderProgramObject, "u_ks"), 1, color.specular_color);

        if (textures.size() > 1)
        {
            glUniform1i(glGetUniformLocation(shaderProgramObject, "normalMappingEnabled"), 1);
        }
        else
        {
            glUniform1i(glGetUniformLocation(shaderProgramObject, "normalMappingEnabled"), 0);
        }

        // draw mesh
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        // PrintLog("static_cast<unsigned int>(indices.size()) : %d \n", static_cast<unsigned int>(indices.size() * 3));

        for (unsigned int i = 0; i < textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }

    void Render_Ktx(GLuint shaderProgramObject)
    {
        // draw mesh
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void RenderParticles(float particleSpeed, BOOL disintegrate, BOOL integrate, int direction, float maxDistance)
    {
        // DisIntegrate
        if (disintegrate && !integrate)
        {
            runCudaDisintegration(true, particleSpeed, direction, maxDistance);
        }
        // Integrate
        else if (integrate)
        {
            runCudaDisintegration(false, particleSpeed, direction, maxDistance);
        }

        glEnable(GL_PROGRAM_POINT_SIZE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // draw mesh
        glBindVertexArray(vaoParticles);
        glDrawArrays(GL_POINTS, 0, particleVertices.size() / 3);
        glBindVertexArray(0);

        glDisable(GL_BLEND);
        glDisable(GL_PROGRAM_POINT_SIZE);
    }

    // render data
    unsigned int vbo, vbo_Indices;
    unsigned int vboParticlesPosition;
    unsigned int vboParticlesColor;
    int N;
    float *initialParticlePosition;
    cudaGraphicsResource_t cudaResourcePosition;
    cudaGraphicsResource_t cudaResourceColor;
    float *deviceInputInitialArray = NULL;

    void initializeModel()
    {

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &vbo_Indices);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_Indices);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(MATRIX_ATTRIBUTE_POSITION);
        glVertexAttribPointer(MATRIX_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);

        glEnableVertexAttribArray(MATRIX_ATTRIBUTE_NORMAL);
        glVertexAttribPointer(MATRIX_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Normal));

        glEnableVertexAttribArray(MATRIX_ATTRIBUTE_TEXTURE0);
        glVertexAttribPointer(MATRIX_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, TexCoords));

        glEnableVertexAttribArray(MATRIX_ATTRIBUTE_TANGENT);
        glVertexAttribPointer(MATRIX_ATTRIBUTE_TANGENT, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Tangent));

        glEnableVertexAttribArray(MATRIX_ATTRIBUTE_BITTANGENT);
        glVertexAttribPointer(MATRIX_ATTRIBUTE_BITTANGENT, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Bitangent));

        glEnableVertexAttribArray(5);
        glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void *)offsetof(Vertex, m_BoneIDs));

        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, m_Weights));
        glBindVertexArray(0);

        // ================================================================== Particles
        if (isDisintegration)
        {

            glGenVertexArrays(1, &vaoParticles);
            glBindVertexArray(vaoParticles);

            // VBO for position
            glGenBuffers(1, &vboParticlesPosition);
            glBindBuffer(GL_ARRAY_BUFFER, vboParticlesPosition);
            glBufferData(GL_ARRAY_BUFFER, particleVertices.size() * sizeof(float), particleVertices.data(), GL_DYNAMIC_DRAW);
            glVertexAttribPointer(MATRIX_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
            glEnableVertexAttribArray(MATRIX_ATTRIBUTE_POSITION);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            // VBO for Color
            N = particleVertices.size() / 3;

            float *vertexColor = (float *)malloc(N * 4 * sizeof(float));
            std::fill_n(vertexColor, N * 4, 1.0f);

            glGenBuffers(1, &vboParticlesColor);
            glBindBuffer(GL_ARRAY_BUFFER, vboParticlesColor);
            glBufferData(GL_ARRAY_BUFFER, N * 4 * sizeof(float), vertexColor, GL_DYNAMIC_DRAW);
            glVertexAttribPointer(MATRIX_ATTRIBUTE_COLOR, 4, GL_FLOAT, GL_FALSE, 0, NULL);
            glEnableVertexAttribArray(MATRIX_ATTRIBUTE_COLOR);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            glBindVertexArray(0);

            // Copying initialPosition
            initialParticlePosition = (float *)malloc(particleVertices.size() * sizeof(float));
            std::copy(particleVertices.data(), particleVertices.data() + particleVertices.size(), initialParticlePosition);

            // Registering cuda buffer
            gpuErrchk(cudaGraphicsGLRegisterBuffer(&cudaResourcePosition, vboParticlesPosition, cudaGraphicsMapFlagsWriteDiscard));
            gpuErrchk(cudaGraphicsGLRegisterBuffer(&cudaResourceColor, vboParticlesColor, cudaGraphicsMapFlagsWriteDiscard));

            cudaMalloc((void **)&deviceInputInitialArray, particleVertices.size() * sizeof(float));
            cudaMemcpy(deviceInputInitialArray, particleVertices.data(), particleVertices.size() * sizeof(float), cudaMemcpyHostToDevice);
        }
    }

    void GenTextureFromData()
    {
        for (int i = 0; i < this->textures.size(); i++)
        {

            unsigned char *data = this->textures[i].data;
            unsigned int textureID;
            glGenTextures(1, &textureID);
            int width = this->textures[i].width;
            int height = this->textures[i].height;
            int nrComponents = this->textures[i].nrComponents;

            if (data != NULL)
            {
                GLenum format;
                if (nrComponents == 1)
                    format = GL_RED;
                else if (nrComponents == 3)
                    format = GL_RGB;
                else if (nrComponents == 4)
                    format = GL_RGBA;
                else
                {
                    this->textures[i].id = 0;
                    return;
                }

                glBindTexture(GL_TEXTURE_2D, textureID);

                if (format == GL_RGBA)
                {
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                }

                else
                {
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                }
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, (void *)data);
                glGenerateMipmap(GL_TEXTURE_2D);

                // stbi_image_free(data);
                this->textures[i].id = textureID;
                return;
            }
            this->textures[i].id = 0;
        }
    }

    void runCudaDisintegration(bool isDisintegrate, float particleSpeed, int direction, float maxDistance)
    {
        cudaGraphicsMapResources(1, &cudaResourcePosition, 0);
        cudaGraphicsMapResources(1, &cudaResourceColor, 0);
        float *cudaPointerPosition;
        float *cudaPointerColor;
        size_t cudaSizePosition;
        size_t cudaSizeColor;

        cudaGraphicsResourceGetMappedPointer((void **)&cudaPointerPosition, &cudaSizePosition, cudaResourcePosition);
        cudaGraphicsResourceGetMappedPointer((void **)&cudaPointerColor, &cudaSizeColor, cudaResourceColor);

        launch_arrayUpdateKernel(cudaPointerPosition, cudaPointerColor, deviceInputInitialArray, isDisintegrate, N, particleSpeed, direction, maxDistance);

        // unmap buffer object
        cudaGraphicsUnmapResources(1, &cudaResourceColor, 0);
        cudaGraphicsUnmapResources(1, &cudaResourcePosition, 0);
    }
};
