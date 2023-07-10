#include "common.h"

mat4 matrixStack[MODEL_VIEW_MATRIX_STACK];
int matrixStackTop = -1;

GLuint FBO_DOF;
// GLuint fbo_UserMap;

// void launch_arrayUpdateKernel(void* cudaPointerPosition, void* cudaPointerColor, float* initialPosition, bool isDisintegrate, int N, float _speed, int direction, float maxDistance)
// {

// }

// void launch_reduceVectorSizeKernel(float* input, float* output, int inputSize, int outputSize)
// {

// }

BOOL LoadPNGImage(GLuint *texture, const char *imageFilePath)
{
    // variable declarations
    int image_width = 0;
    int image_height = 0;
    int numImageComponents = 0;
    unsigned char *image_data = NULL;
    GLenum image_format = 0;

    // code
    glGenTextures(1, texture);
    image_data = stbi_load(imageFilePath, &image_width, &image_height, &numImageComponents, 0);
    if (image_data != NULL)
    {
        if (numImageComponents == 1)
        {
            image_format = GL_RED;
        }
        else if (numImageComponents == 3)
        {
            image_format = GL_RGB;
        }
        else if (numImageComponents == 4)
        {
            image_format = GL_RGBA;
        }

        glBindTexture(GL_TEXTURE_2D, *texture);

        if (image_format == GL_RGBA)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }

        else
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        }

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, image_format, image_width, image_height, 0, image_format, GL_UNSIGNED_BYTE, (void *)image_data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(image_data);
        return (TRUE);
    }
    return (FALSE);
}

BOOL LoadGLTexture(GLuint *texture, TCHAR ImageResourceID[])
{
    // variable declartions
    HBITMAP hBitmap = NULL;
    BITMAP bmp;
    BOOL bResult = FALSE;

    // code
    hBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), ImageResourceID, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);

    if (hBitmap)
    {
        bResult = TRUE;
        GetObject(hBitmap, sizeof(BITMAP), &bmp);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // 4 TO 1 FOR BETTER PERFORMANCE

        glGenTextures(1, texture);

        glBindTexture(GL_TEXTURE_2D, *texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // create the texture
        glTexImage2D(GL_TEXTURE_2D,    // Targter
                     0,                // MipMap Level (done by opengl)
                     GL_RGB,           // Opengl Image format
                     bmp.bmWidth,      // Image Width
                     bmp.bmHeight,     // Image Height
                     0,                // Border Width
                     GL_BGR,           // Image Format
                     GL_UNSIGNED_BYTE, // Data type of bmp.bmBits
                     bmp.bmBits);      //

        glGenerateMipmap(GL_TEXTURE_2D); // Generate MipMap

        glBindTexture(GL_TEXTURE_2D, 0); // unbind texture

        // DELETE Object
        DeleteObject(hBitmap);
    }
    return bResult;
}

BOOL Load1DPNGImage(GLuint *texture, float *data)
{
    // variable declarations
    int image_width = 0;
    int image_height = 0;
    int numImageComponents = 0;
    unsigned char *image_data = NULL;
    GLenum image_format = 0;

    // code
    glGenTextures(1, texture);
    // image_data = stbi_load(imageFilePath, &image_width, &image_height, &numImageComponents, 0);
    // if (image_data != NULL)
    {
        // if (numImageComponents == 1)
        //     image_format = GL_RED;
        // else if (numImageComponents == 3)
        //     image_format = GL_RGB;
        // else if (numImageComponents == 4)
        //     image_format = GL_RGBA;

        glBindTexture(GL_TEXTURE_1D, *texture);

        if (image_format == GL_RGBA)
        {
            glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }

        else
        {
            glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        }

        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        // glTexImage2D(GL_TEXTURE_2D, 0, image_format, image_width, image_height, 0, image_format, GL_UNSIGNED_BYTE, (void *)image_data);
        
         glTexImage1D( GL_TEXTURE_1D,
                        0,
                        GL_R32F,
                        370,
                        0,
                        GL_RED,
                        GL_FLOAT,
                        (void *)data);
        // stbi_image_free(image_data);
        glGenerateMipmap(GL_TEXTURE_1D);
        return (TRUE);
    }
    return (FALSE);
}


unsigned int loadCubemap(const char *faces[])
{
    unsigned int textureID;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < 6; i++)
    {
        // PrintLog("FIle name = %s\n", faces[i]);
        unsigned char *data = stbi_load(faces[i], &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
            // PrintLog("Cubemap tex Success to load at path: %s\n", faces[i]);
        }
        else
        {
            // std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            // PrintLog("Cubemap tex failed to load at path: %s\n", faces[i]);
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    return textureID;
}

void initializeMatrixStack(void)
{
    // code
    matrixStackTop = 0;
    for (int i = matrixStackTop; i < MODEL_VIEW_MATRIX_STACK; i++)
        matrixStack[i] = mat4::identity();
}

void pushMatrix(mat4 matrix)
{
    // Function Prototype
    void uninitialize(void);

    //	Code
    if (matrixStackTop > (MODEL_VIEW_MATRIX_STACK - 1))
    {
        PrintLog("ERROR - EXCEEDED MATRIX STACK LIMIT:\n");
        // uninitialize();
    }

    matrixStack[matrixStackTop] = matrix;
    matrixStackTop++;
}

mat4 popMatrix(void)
{
    // function ptototype
    void uninitialize(void);

    // variable declartions
    mat4 matrix;

    // code
    if (matrixStackTop < 0)
    {
        PrintLog("ERROR : MATRIX STACK EMPTY!\n");
        // uninitialize();
    }

    matrixStack[matrixStackTop] = mat4::identity();
    matrixStackTop--;

    matrix = matrixStack[matrixStackTop];

    return (matrix);
}

float degToRad(float degrees)
{
    return (degrees * M_PI / 180.0);
}

float osc(float time, float minRange, float maxRange, float frequency)
{
    float amplitude = (maxRange - minRange) / 2.0;
    float offset = minRange + amplitude;

    return amplitude * sin(2 * M_PI * frequency * time) + offset;
}
// float getXOSC(float minVal, float maxVal)
// {
//    return osc(time, minVal, maxVal, 1.5);
// }
//  void launch_arrayUpdateKernel(void *cudaPointerPosition, void *cudaPointerColor, float *initialPosition, bool isDisintegrate, int N, float _speed, int direction, float maxDistance)
//  {
//  }
//  void launch_reduceVectorSizeKernel(float *input, float *output, int inputSize, int outputSize)
//  {
//  }