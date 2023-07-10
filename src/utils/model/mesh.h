#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include <GL/glew.h> // This must be before gl.h
#include <GL/gl.h>
#include <stdlib.h>
#include <stdio.h>   // For fopen_s(), f//PrintLog(), fclose()
#include <windows.h> // For BOOL, TEXT(), MessageBox(), etc.cd

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "../common.h"

// MACROS
#define BUFFER_SIZE 1024

// TYPEDEFS
typedef struct VEC_INT vec_int_T;
typedef struct VEC_FLOAT vec_float_T;
typedef struct mesh_data mesh_data_T;

// GLOBAL VARIABLES
static FILE *gp_mesh_file;
static char buffer[BUFFER_SIZE];

struct VEC_INT
{
    GLint *p_arr;
    int size;
};

struct VEC_FLOAT
{
    GLfloat *p_arr;
    int size;
};

// struct Mesh
// {
//     std::vector<struct mesh_data *> mesh;
// };

struct mesh_data
{
    // GEOMETRY DATA
    vec_float_T *gp_vertex;
    vec_float_T *gp_texture;
    vec_float_T *gp_normal;

    vec_float_T *gp_vertex_filter;
    vec_float_T *gp_texture_filter;
    vec_float_T *gp_normal_filter;

    vec_int_T *gp_vertex_indices;
    vec_int_T *gp_texture_indices;
    vec_int_T *gp_normal_indices;

    // TEXTURE DATA
    GLuint diffuse_texture;
    GLuint normal_texture;
};

// FUNCTION PROTOTYPE - MESH RELATED
mesh_data_T *load_mesh(const char *);
void free_mesh_data(mesh_data_T **);

// FUNCTION PROTOTYPE - VECTOR RELATED
mesh_data_T *create_mesh_data(void);
vec_int_T *create_vec_int(void);
vec_float_T *create_vec_float(void);

void push_back_vec_int(vec_int_T *, GLint);
void push_back_vec_float(vec_float_T *, GLfloat);

void show_vec_int(vec_int_T *);
void show_vec_float(vec_float_T *);

void destroy_vec_int(vec_int_T *);
void destroy_vec_float(vec_float_T *);

/// Implimentatio using STL
class Face
{
public:
    int *vetrices;
    int *textures;
    int *normals;

    Face(int *v, int *t, int *n)
    {
        this->vetrices = v;
        this->textures = t;
        this->normals = n;
    }
    ~Face()
    {
        delete this->vetrices;
        delete this->textures;
        delete this->normals;
    }
};

class new_mtl
{
public:
    std::string name;

    std::vector<float> ambient_color;  // Ka
    std::vector<float> diffuse_color;  // Kd
    std::vector<float> specular_color; // Ks

    GLuint diffue_texture = 0;
    GLuint normal_texture = 0;

    new_mtl(std::string _name) : name(_name)
    {
    }
};

class new_obj
{

public:
    float tem[3] = {0.0f, 0.0f, 0.0f};
    std::vector<float *> vertices;
    std::vector<float *> texcoords;
    std::vector<float *> normals;
    std::vector<Face *> faces;
    int face_size = 0;

    std::string name;
    std::string use_mtl;
    BOOL hasTexCoords = FALSE;

    std::vector<float> vertecesAfterFilter();
    std::vector<float> texCoordAfterFilter();
    std::vector<float> normalAfterFilter();

    //
    GLuint vao_Model;
    GLuint vbo_Model_Position;
    GLuint vbo_Model_Normal;
    GLuint vbo_Model_Texcoord;

    // Matrial Data
    new_mtl *texture_data;

    new_obj(std::string _name) : name(_name)
    {
    }
};

class STL_MODELLOADING
{

public:
    STL_MODELLOADING()
    {
    }

    BOOL loadMTL(const char *fileName)
    {
        std::string line;
        std::vector<std::string> lines;

        std::ifstream in(fileName);
        if (!in.is_open())
        {
            printf("Cannot load model %s\n", fileName);
            return FALSE;
        }

        while (!in.eof())
        {
            std::getline(in, line);
            lines.push_back(line);
        }
        in.close();

        // logic
        float a, b, c;
        char *str;

        for (int i = 0; i < lines.size(); i++)
        {
            line = lines[i];

            // if (line[0] == 'n' && line[1] == 'e' && line[2] == 'w') // new mtl
            if ((line.compare("newmtl")) > 0) // new mtl
            {
                material.push_back(new new_mtl(line.c_str()));
                for (int i = 0; i < 3; i++)
                    material[material.size() - 1]->ambient_color.push_back(1.0f);
                for (int i = 0; i < 3; i++)
                    material[material.size() - 1]->diffuse_color.push_back(1.0f);
                for (int i = 0; i < 3; i++)
                    material[material.size() - 1]->specular_color.push_back(1.0f);
            }
            else if (line[0] == 'K')
            {
                if (line[1] == 'a') // map_Kd diffuse map
                {
                    PrintLog("%s\n", line.c_str());
                    sscanf(line.c_str(), "Ka %f %f %f", &a, &b, &c);
                    material[material.size() - 1]->ambient_color[0] = a;
                    material[material.size() - 1]->ambient_color[1] = b;
                    material[material.size() - 1]->ambient_color[2] = c;
                }
                if (line[1] == 'd') // map_Kd diffuse map
                {
                    PrintLog("%s\n", line.c_str());
                    sscanf(line.c_str(), "Kd %f %f %f", &a, &b, &c);
                    material[material.size() - 1]->diffuse_color[0] = a;
                    material[material.size() - 1]->diffuse_color[1] = b;
                    material[material.size() - 1]->diffuse_color[2] = c;
                }
                else if (line[1] == 's') // Ke
                {
                    PrintLog("%s\n", line.c_str());
                    sscanf(line.c_str(), "Ks %f %f %f", &a, &b, &c);
                    material[material.size() - 1]->specular_color[0] = a;
                    material[material.size() - 1]->specular_color[1] = b;
                    material[material.size() - 1]->specular_color[2] = c;
                }
            }
            else if (line[0] == 'm')
            {
                GLuint texture = 0;

                if (line[4] == 'K' && line[5] == 'd') // map_Kd diffuse map
                {

                    int len = line.length();
                    str = (char *)malloc(sizeof(char) * len);

                    sscanf(line.c_str(), "map_Kd %s", str);
                    PrintLog("%s\n", str);

                    if (LoadPNGImage(&texture, str) == FALSE)
                    {
                        PrintLog("Diffuse Texture Failed For %s Model. Please Give Correct Path of textures in .mtl File.\n", fileName);
                    }

                    material[material.size() - 1]->diffue_texture = texture;
                    free(str);
                    str = NULL;
                }
                else if (line[4] == 'K' && line[5] == 's') // map_Kd diffuse map
                {

                    int len = line.length();
                    str = (char *)malloc(sizeof(char) * len);

                    sscanf(line.c_str(), "map_Kd %s", str);

                    if (LoadPNGImage(&texture, str) == FALSE)
                    {
                        PrintLog("Normal Texture Failed For %s Model. Please Give Correct Paths of textures in .mtl File.\n", fileName);
                    }
                    material[material.size() - 1]->normal_texture = texture;
                    free(str);
                    str = NULL;
                }
            }
        }

        lines.clear();

        // printmtl();

        return TRUE;
    }

    void printmtl(void)
    {
        PrintLog("\nn========================================\n\n");
        PrintLog("d material.size() : %d\n\n", material.size());
        // PrintLog("ambient_color : %f %f %f\n", material[0]->ambient_color[0], material[0]->ambient_color[1], material[0]->ambient_color[2]);

        for (int i = 0; i < material.size(); i++)
        {
            if (material[i])
            {

                PrintLog("name : %s \n", material[i]->name);
                PrintLog("ambient_color : %f %f %f\n", material[i]->ambient_color[0], material[i]->ambient_color[1], material[i]->ambient_color[2]);
                PrintLog("diffuse_color : %f %f %f\n", material[i]->diffuse_color[0], material[i]->diffuse_color[1], material[i]->diffuse_color[2]);
                PrintLog("specular_color : %f %f %f\n", material[i]->specular_color[0], material[i]->specular_color[1], material[i]->specular_color[2]);
                PrintLog("diffue_texture : %d\n", material[i]->diffue_texture);
                PrintLog("normal_texture : %d\n\n", material[i]->normal_texture);
            }
        }

        PrintLog("\nn========================================\n\n");
    }

    std::vector<new_obj *> obj;
    std::vector<new_mtl *> material;

    BOOL loadModel(const char *fileName)
    {
        PrintLog("BOOL loadModel(const char *fileName)\n");

        std::string line;
        std::vector<std::string> lines;

        std::ifstream in(fileName);
        if (!in.is_open())
        {
            printf("Cannot load model %s\n", fileName);
            return FALSE;
        }

        while (!in.eof())
        {
            std::getline(in, line);
            lines.push_back(line);
        }
        in.close();

        float a, b, c;
        int temp = 0;
        for (int i = 0; i < lines.size(); i++)
        {
            line = lines[i];
            // if ((line.compare("newmtl")) > 0) // new mtl
            if (line[0] == 'o') // new mtl
            {
                PrintLog("temp %d\n", temp);
                temp++;
                if (!obj.empty())
                {
                    obj[obj.size() - 1]->face_size = obj[obj.size() - 1]->faces.size();
                }

                // if (!obj.empty())
                obj.push_back(new new_obj(line.c_str()));
            }
            else if (line[0] == 'u') // new mtl
            {
                obj[obj.size() - 1]->use_mtl = line.c_str();
            }
            else if (line[0] == 'v')
            {
                if (line[1] == ' ')
                {
                    sscanf(line.c_str(), "v %f %f %f", &a, &b, &c);
                    obj[obj.size() - 1]->vertices.push_back(new float[3]{a, b, c});
                    // obj[obj.size() - 1]->vertices.push_back(new float[3]{a, b, c});
                }
                else if (line[1] == 't')
                {
                    sscanf(line.c_str(), "vt %f %f", &a, &b);

                    obj[obj.size() - 1]->texcoords.push_back(new float[2]{a, b});
                }
                else if (line[1] == 'n')
                {
                    sscanf(line.c_str(), "vn %f %f %f", &a, &b, &c);

                    obj[obj.size() - 1]->normals.push_back(new float[3]{a, b, c});
                    // normals.push_back(new float[3]{a, b, c});
                }
            }
            else if (line[0] == 'f')
            {
                int v0, v1, v2, v3, t0, t1, t2, t3, n0, n1, n2, n3;
                sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d ", &v0, &t0, &n0, &v1, &t1, &n1, &v2, &t2, &n2);

                int *v = new int[3]{v0 - 1, v1 - 1, v2 - 1};
                int *t = new int[3]{t0 - 1, t1 - 1, t2 - 1};
                int *n = new int[3]{n0 - 1, n1 - 1, n2 - 1};

                obj[obj.size() - 1]->faces.push_back(new Face(v, t, n));
                // faces.push_back(new Face(v, t, n));
            }
        }
        lines.clear();
        filterVertices();
        return TRUE;
    }

    void filterVertices()
    {

        // for (std::vector<new_obj *>::iterator iter = obj.begin(); iter < obj.end(); iter++)
        PrintLog("\n obj.size() : %d\n\n", obj.size());
        for (int k = 0; k < obj.size(); k++)
        {
            // obj[k]->vertecesAfterFilter = std::vector<float>;

            PrintLog("obj[k]->face_size %d\n", obj[k]->face_size);
            for (int i = 0; i < obj[k]->face_size; i++)
            {
                for (int j = 0; j < 3; j++)
                {

                    int vi = obj[k]->faces.at(i)->vetrices[j];
                    int ti = obj[k]->faces.at(i)->textures[j];
                    int ni = obj[k]->faces.at(i)->normals[j];
                    // if (obj[k]->texcoords.empty() && obj[k]->texcoords[ti] != NULL)
                    // if (ti != NULL && !obj[k]->texcoords.empty() && obj[k]->texcoords[ti] != NULL)
                    // {
                    //     obj[k]->texCoordAfterFilter.push_back(obj[k]->texcoords[ti][0]);
                    //     obj[k]->texCoordAfterFilter.push_back(obj[k]->texcoords[ti][1]);
                    //     obj[k]->hasTexCoords = TRUE;
                    // }
                    auto tmp = obj[k]->vertices[vi][0];
                    tmp = obj[k]->vertices[vi][1];
                    tmp = obj[k]->vertices[vi][2];
                    // PrintLog("%f  , %f, %f \n", obj[k]->vertices[vi][0], obj[k]->vertices[vi][1], obj[k]->vertices[vi][2]);
                    // if (!obj[k]->vertices.empty() && obj[k]->vertices[vi] != NULL)
                    // {
                    // obj[k]->vertecesAfterFilter.push_back(obj[k]->vertices[vi][0]);
                    // obj[k]->vertecesAfterFilter.push_back(obj[k]->vertices[vi][1]);
                    // obj[k]->vertecesAfterFilter.push_back(obj[k]->vertices[vi][2]);
                    // }

                    // if (!obj[k]->normals.empty() && obj[k]->normals[ni] != NULL)
                    // {
                    //     obj[k]->normalAfterFilter.push_back(obj[k]->normals[ni][0]);
                    //     obj[k]->normalAfterFilter.push_back(obj[k]->normals[ni][1]);
                    //     obj[k]->normalAfterFilter.push_back(obj[k]->normals[ni][2]);
                    // }
                    // PrintLog("%f %f %f \n", obj[k]->texcoords[ti][0], obj[k]->texcoords[ti][0], obj[k]->texcoords[ti][0]);
                }
            }
            // glGenVertexArrays(1, &(obj[k]->vao_Model));
            // glBindVertexArray(obj[k]->vao_Model);

            // // VBO for position
            // glGenBuffers(1, &(obj[k]->vbo_Model_Position));
            // glBindBuffer(GL_ARRAY_BUFFER, obj[k]->vbo_Model_Position);
            // // glBufferData(GL_ARRAY_BUFFER, mesh_data->gp_vertex_filter->size * sizeof(float), mesh_data->gp_vertex_filter->p_arr, GL_STATIC_DRAW);
            // // if (!obj[k]->vertecesAfterFilter.empty() && (&obj[k]->vertecesAfterFilter.at(0) )
            // //     glBufferData(GL_ARRAY_BUFFER, obj[k]->vertecesAfterFilter.size() * sizeof(float), &obj[k]->vertecesAfterFilter.at(0), GL_STATIC_DRAW);
            // glVertexAttribPointer(MATRIX_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
            // glEnableVertexAttribArray(MATRIX_ATTRIBUTE_POSITION);
            // glBindBuffer(GL_ARRAY_BUFFER, 0);

            // // // VBO for Texcoord
            // // glGenBuffers(1, &vbo_Model_Texcoord);
            // // glBindBuffer(GL_ARRAY_BUFFER, vbo_Model_Texcoord);
            // // // glBufferData(GL_ARRAY_BUFFER, mesh_data->gp_texture_filter->size * sizeof(float), mesh_data->gp_texture_filter->p_arr, GL_STATIC_DRAW);
            // // glBufferData(GL_ARRAY_BUFFER, model.texCoordAfterFilter.size() * sizeof(float), &model.texCoordAfterFilter[0], GL_STATIC_DRAW);
            // // glVertexAttribPointer(MATRIX_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
            // // glEnableVertexAttribArray(MATRIX_ATTRIBUTE_TEXTURE0);
            // // glBindBuffer(GL_ARRAY_BUFFER, 0);

            // // // VBO for Normal
            // // glGenBuffers(1, &vbo_Model_Normal);
            // // glBindBuffer(GL_ARRAY_BUFFER, vbo_Model_Normal);
            // // // glBufferData(GL_ARRAY_BUFFER, mesh_data->gp_normal_filter->size * sizeof(float), mesh_data->gp_normal_filter->p_arr, GL_STATIC_DRAW);
            // // glBufferData(GL_ARRAY_BUFFER, model.normalAfterFilter.size() * sizeof(float), &model.normalAfterFilter[0], GL_STATIC_DRAW);
            // // glVertexAttribPointer(MATRIX_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
            // // glEnableVertexAttribArray(MATRIX_ATTRIBUTE_NORMAL);
            // // glBindBuffer(GL_ARRAY_BUFFER, 0);

            // // // VBO for Normal
            // // glGenBuffers(1, &vbo_Matrices);
            // // glBindBuffer(GL_ARRAY_BUFFER, vbo_Matrices);
            // // glBufferData(GL_ARRAY_BUFFER, 9.0f * sizeof(float), NULL, GL_DYNAMIC_DRAW);
            // // glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, NULL);
            // // glEnableVertexAttribArray(4);
            // // glBindBuffer(GL_ARRAY_BUFFER, 0);

            // glBindVertexArray(0);
        }
    }

    std::string
    retrieveString(char *buf)
    {

        size_t len = 0;
        while ((buf[len] != '\0'))
        {
            len++;
        }

        return std::string(buf, len);
    }
};
