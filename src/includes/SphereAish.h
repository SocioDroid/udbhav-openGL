#pragma once
#include "vmath.h"
#include "3DModels.h"
using namespace vmath;

const int MAX_VERTICES_SPHERE = 100000;
const int MAX_TEXCOORDS_SPHERE = 100000;
const int MAX_NORMALS_SPHERE = 100000;
const int CACHE_SIZE_SPHERE = 240;

class SphereAish
{
public:
    // variables
    float model_vertices[MAX_VERTICES];
    float model_textures[MAX_TEXCOORDS];
    float model_normals[MAX_NORMALS];
    int texturePointer = 0;
    int verticesPointer = 0;
    int normalsPointer = 0;

    // methods

    SphereAish(float radius, int slices, int stacks)
    {
        // code
        generateSphereData(radius, slices, stacks);
    }

    void addSphereVertex(double x, double y, double z)
    {
        model_vertices[verticesPointer++] = (float)x;
        model_vertices[verticesPointer++] = (float)y;
        model_vertices[verticesPointer++] = (float)z;
    }

    void addSphereNormal(double x, double y, double z)
    {
        model_normals[normalsPointer++] = (float)x;
        model_normals[normalsPointer++] = (float)y;
        model_normals[normalsPointer++] = (float)z;
    }

    void addSphereTexCoords(double a, double b)
    {
        model_textures[texturePointer++] = (float)a;
        model_textures[texturePointer++] = (float)b;
    }

    float *getSphereVertex()
    {
        return model_vertices;
    }

    float *getSphereTexCoord()
    {
        return model_textures;
    }
    float *getSphereNormals()
    {
        return model_normals;
    }

    // Function which generates sphere coordinates
    void generateSphereData(float radius, int slices, int stacks)
    {
        int i, j;
        float sinCache1a[CACHE_SIZE];
        float cosCache1a[CACHE_SIZE];
        float sinCache2a[CACHE_SIZE];
        float cosCache2a[CACHE_SIZE];
        float sinCache1b[CACHE_SIZE];
        float cosCache1b[CACHE_SIZE];
        float sinCache2b[CACHE_SIZE];
        float cosCache2b[CACHE_SIZE];
        float angle;
        float zLow, zHigh;
        float sintemp1 = 0.0f, sintemp2 = 0.0f, sintemp3 = 0.0f, sintemp4 = 0.0f;
        float costemp3 = 0.0f, costemp4 = 0.0f;
        int start, finish;

        if (slices >= CACHE_SIZE)
            slices = CACHE_SIZE - 1;
        if (stacks >= CACHE_SIZE)
            stacks = CACHE_SIZE - 1;
        if (slices < 2 || stacks < 1 || radius < 0.0)
        {
            return;
        }

        for (i = 0; i < slices; i++)
        {
            angle = 2 * (float)M_PI * i / slices;
            sinCache1a[i] = (float)sin(angle);
            cosCache1a[i] = (float)cos(angle);
            sinCache2a[i] = sinCache1a[i];
            cosCache2a[i] = cosCache1a[i];
        }

        for (j = 0; j <= stacks; j++)
        {
            angle = (float)M_PI * j / stacks;
            sinCache2b[j] = (float)sin(angle);
            cosCache2b[j] = (float)cos(angle);
            sinCache1b[j] = radius * (float)sin(angle);
            cosCache1b[j] = radius * (float)cos(angle);
        }
        /* Make sure it comes to a point */
        sinCache1b[0] = 0;
        sinCache1b[stacks] = 0;

        sinCache1a[slices] = sinCache1a[0];
        cosCache1a[slices] = cosCache1a[0];
        sinCache2a[slices] = sinCache2a[0];
        cosCache2a[slices] = cosCache2a[0];

        start = 0;
        finish = stacks;
        for (j = start; j < finish; j++)
        {
            zLow = cosCache1b[j];
            zHigh = cosCache1b[j + 1];
            sintemp1 = sinCache1b[j];
            sintemp2 = sinCache1b[j + 1];

            sintemp3 = sinCache2b[j + 1];
            costemp3 = cosCache2b[j + 1];
            sintemp4 = sinCache2b[j];
            costemp4 = cosCache2b[j];

            for (i = 0; i <= slices; i++)
            {
                addSphereNormal(sinCache2a[i] * sintemp3, cosCache2a[i] * sintemp3, costemp3);
                addSphereTexCoords(1 - (float)i / slices, 1 - (float)(j + 1) / stacks);
                addSphereVertex(sintemp2 * sinCache1a[i], sintemp2 * cosCache1a[i], zHigh);

                addSphereNormal(sinCache2a[i] * sintemp4, cosCache2a[i] * sintemp4, costemp4);
                addSphereTexCoords(1 - (float)i / slices, 1 - (float)j / stacks);
                addSphereVertex(sintemp1 * sinCache1a[i], sintemp1 * cosCache1a[i], zLow);
            }
        }
    }

    int getNumberOfSphereVertices()
    {
        // code
        return (verticesPointer / 3);
    }

    int getNumberOfSphereTexcoords()
    {
        // code
        return texturePointer;
    }

    int getNumberOfSphereNormals()
    {
        // code
        return normalsPointer;
    }
};
