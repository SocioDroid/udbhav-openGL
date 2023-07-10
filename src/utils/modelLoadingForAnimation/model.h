#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>

#include "../common.h"
#include "mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../includes/stb_image.h"

using namespace std;

class Model
{
public:
    // model data
    vector<Texture> textures_loaded;
    vector<Mesh> meshes;
    vector<float> particleVertices;

    string directory;
    bool gammaCorrection;
    bool isDisintegration;
    int maxMeshVertices;
    // Default Constructor
    Model() {}

    Model(bool isDisintegration, int maxMeshVertices)
    {
        this->isDisintegration = isDisintegration;
        this->maxMeshVertices = maxMeshVertices;
    }

    Model(string const &path, bool gamma = false) : gammaCorrection(gamma)
    {
        loadModel(path);
    }
    // To run by the thread
    void initializeModel(string const &path)
    {
        gammaCorrection = false;
        loadModel(path);
    }

    // To run by the main thread
    void initializeMeshTextures()
    {
        // for (int i = 0; i < meshes.size(); i++)
        // {
        //     meshes[i].GenTextureFromData();
        // }
    }
    void initializeMesh()
    {
        for (int i = 0; i < meshes.size(); i++)
        {
            meshes[i].initializeModel();
        }
    }
    // =========================
    void Render(GLuint shaderProgramObject)
    {
        for (unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].Render(shaderProgramObject);
    }
    void RenderParticles(float particleSpeed, BOOL disintegrate, BOOL integrate, int direction, float maxDistance)
    {
        for (unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].RenderParticles(particleSpeed, disintegrate, integrate, direction, maxDistance);
    }

    void Render_Ktx(GLuint shaderProgramObject)
    {
        meshes[0].Render_Ktx(shaderProgramObject);
    }

private:
    void loadModel(string const &path)
    {
        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            PrintLog("ERROR::ASSIMP:: Failed To Load %s Model.\n", path.c_str());
            return;
        }
        // PrintLog("path : %s\n", path.c_str());
        directory = path.substr(0, path.find_last_of('\\'));
        // PrintLog("directory: %s\n", directory.c_str());

        processNode(scene->mRootNode, scene);
    }

    void processNode(aiNode *node, const aiScene *scene)
    {
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }

        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
    }

    Mesh processMesh(aiMesh *mesh, const aiScene *scene)
    {
        vector<Vertex> vertices;
        vector<unsigned int> indices;
        vector<Texture> textures;
        Color color;

        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;

            vmath::vec3 vector; // positions
            vector[0] = mesh->mVertices[i].x;
            vector[1] = mesh->mVertices[i].y;
            vector[2] = mesh->mVertices[i].z;
            vertex.Position = vector;

            particleVertices.push_back(mesh->mVertices[i].x);
            particleVertices.push_back(mesh->mVertices[i].y);
            particleVertices.push_back(mesh->mVertices[i].z);

            // normals
            if (mesh->HasNormals())
            {
                vector[0] = mesh->mNormals[i].x;
                vector[1] = mesh->mNormals[i].y;
                vector[2] = mesh->mNormals[i].z;
                vertex.Normal = vector;
            }
            // texture coordinates
            if (mesh->mTextureCoords[0])
            {
                vmath::vec2 vec;

                vec[0] = mesh->mTextureCoords[0][i].x;
                vec[1] = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
                // tangent
                vector[0] = mesh->mTangents[i].x;
                vector[1] = mesh->mTangents[i].y;
                vector[2] = mesh->mTangents[i].z;
                vertex.Tangent = vector;
                // bitangent
                vector[0] = mesh->mBitangents[i].x;
                vector[1] = mesh->mBitangents[i].y;
                vector[2] = mesh->mBitangents[i].z;
                vertex.Bitangent = vector;
            }
            else
                vertex.TexCoords = vmath::vec2(0.0f, 0.0f);

            vertices.push_back(vertex);
        }

        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];

            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

        vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        // specular maps
        vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        // normal maps
        std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        // height maps
        std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

        // read light colors
        vmath::vec3 tmp_color = vmath::vec3(0.8f, 0.8f, 0.8f);
        aiColor4D diffuse;
        if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
        {
            tmp_color[0] = diffuse.r;
            tmp_color[1] = diffuse.g;
            tmp_color[2] = diffuse.b;
        }
        color.diffus_color[0] = tmp_color[0];
        color.diffus_color[1] = tmp_color[1];
        color.diffus_color[2] = tmp_color[2];
        // PrintLog("diffus_color : %f , %f, %f\n", tmp_color[0], tmp_color[1], tmp_color[2]);

        // ambiant
        tmp_color = vmath::vec3(0.0f, 0.0f, 0.0f);
        if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &diffuse))
        {
            tmp_color[0] = diffuse.r;
            tmp_color[1] = diffuse.g;
            tmp_color[2] = diffuse.b;
        }
        color.ambiant_color[0] = tmp_color[0];
        color.ambiant_color[1] = tmp_color[1];
        color.ambiant_color[2] = tmp_color[2];
        // PrintLog("ambiant_color : %f , %f, %f\n", tmp_color[0], tmp_color[1], tmp_color[2]);

        // Specular
        tmp_color = vmath::vec3(0.5f, 0.5f, 0.5f);
        if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &diffuse))
        {
            tmp_color[0] = diffuse.r;
            tmp_color[1] = diffuse.g;
            tmp_color[2] = diffuse.b;
        }
        color.specular_color[0] = tmp_color[0];
        color.specular_color[1] = tmp_color[1];
        color.specular_color[2] = tmp_color[2];
        // PrintLog("specular_color : %f , %f, %f\n", tmp_color[0], tmp_color[1], tmp_color[2]);

        if (isDisintegration)
        {
            std::vector<float> outputVec(maxMeshVertices * 3);
            reduceVectorSize(particleVertices, outputVec, maxMeshVertices);
            particleVertices = outputVec;
            return Mesh(vertices, indices, textures, color, particleVertices);
        }

        return Mesh(vertices, indices, textures, color);
    }

    vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName)
    {
        vector<Texture> textures;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);

            bool skip = false;
            for (unsigned int j = 0; j < textures_loaded.size(); j++)
            {
                if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
                {
                    textures.push_back(textures_loaded[j]);
                    skip = true;
                    break;
                }
            }
            if (!skip)
            {
                Texture texture;
                texture.id = 0;
                TextureFromFile(str.C_Str(), this->directory, true, &texture);

                texture.type = typeName;
                texture.path = str.C_Str();
                textures.push_back(texture);
                textures_loaded.push_back(texture);
            }
        }

        return textures;
    }

    void TextureFromFile(const char *path, const string &directory, bool gamma, Texture *texture)
    {

        string filename = string(path);
        filename = directory + '/' + filename;

        PrintLog("Texture : %s\n", filename.c_str());

        unsigned int textureID;
        glGenTextures(1, &textureID);

        int width, height, nrComponents;
        unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
        if (data)
        {
            GLenum format;
            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else if (nrComponents == 4)
                format = GL_RGBA;

            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        }
        else
        {
            PrintLog("Texture failed to load at path: %s\n", path);

            stbi_image_free(data);
        }

        texture->id = textureID;
    }

    void reduceVectorSize(std::vector<float> &input, std::vector<float> &output, int outputVerticesSize)
    {
        int inputSize = input.size();
        if (outputVerticesSize >= (inputSize / 3))
        {
            // No reduction needed
            output = input;
            return;
        }

        float *d_input;
        float *d_output;
        cudaMalloc((void **)&d_input, inputSize * sizeof(float));
        cudaMalloc((void **)&d_output, outputVerticesSize * 3 * sizeof(float));

        cudaMemcpy(d_input, input.data(), inputSize * sizeof(float), cudaMemcpyHostToDevice);

        // Copy the result back from GPU to CPU
        cudaMemcpy(output.data(), d_output, outputVerticesSize * 3 * sizeof(float), cudaMemcpyDeviceToHost);

        // for (int i = 0; i < output.size(); ++i)
        // {
        //     PrintLog("%f\n", output[i]);
        // }
        cudaFree(d_input);
        cudaFree(d_output);
    }
};
