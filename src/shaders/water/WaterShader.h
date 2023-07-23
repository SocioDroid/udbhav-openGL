#pragma once
#include <string>
#include "../../utils/OpenGL/GLHeadersAndMacros.h"
#include "../../utils/OpenGL/GLShaders.h"
#include "../../includes/vmath.h"
using namespace vmath;

class WaterShader
{
public:
    // Variables
    /* SKY RELATED VARIALBES*/
    GLuint shaderProgramObject;

    BOOL initialize(void)
    {
        // vertex Shader
        GLuint vertexShaderObject = CreateAndCompileShaderObjects(".\\src\\shaders\\water\\water.vs", VERTEX);

        // fragment Shader
        GLuint fragmentShaderObject = CreateAndCompileShaderObjects(".\\src\\shaders\\water\\water.fs", FRAGMENT);

        // Shader Program Object
        shaderProgramObject = glCreateProgram();
        glAttachShader(shaderProgramObject, vertexShaderObject);
        glAttachShader(shaderProgramObject, fragmentShaderObject);

        // link
        BOOL bShaderLinkStatus = LinkShaderProgramObject(shaderProgramObject);

        if (bShaderLinkStatus == FALSE)
            return FALSE;

        return TRUE;
    }

    // utility uniform functions
    // ------------------------------------------------------------------------
    void setBool(const std::string &name, bool value) const
    {
        glUniform1i(glGetUniformLocation(shaderProgramObject, name.c_str()), (int)value);
    }
    // ------------------------------------------------------------------------
    void setInt(const std::string &name, int value) const
    {
        glUniform1i(glGetUniformLocation(shaderProgramObject, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setFloat(const std::string &name, float value) const
    {
        glUniform1f(glGetUniformLocation(shaderProgramObject, name.c_str()), value);
    }
    void setVec2(const std::string &name, vmath::vec2 vector) const
    {
        unsigned int location = glGetUniformLocation(shaderProgramObject, name.c_str());
        glUniform2fv(location, 1, vector);
    }
    void setVec3(const std::string &name, vmath::vec3 vector) const
    {
        unsigned int location = glGetUniformLocation(shaderProgramObject, name.c_str());
        glUniform3fv(location, 1, vector);
    }

    void setVec4(const std::string &name, vmath::vec4 vector) const
    {
        unsigned int location = glGetUniformLocation(shaderProgramObject, name.c_str());
        glUniform4fv(location, 1, vector);
    }

    void setMat4(const std::string &name, vmath::mat4 matrix) const
    {
        unsigned int mat = glGetUniformLocation(shaderProgramObject, name.c_str());
        glUniformMatrix4fv(mat, 1, false, matrix);
    }

    void setSampler2D(const std::string &name, unsigned int texture, int id) const
    {
        glActiveTexture(GL_TEXTURE0 + id);
        glBindTexture(GL_TEXTURE_2D, texture);
        this->setInt(name, id);
    }
    void setSampler3D(const std::string &name, unsigned int texture, int id) const
    {
        glActiveTexture(GL_TEXTURE0 + id);
        glBindTexture(GL_TEXTURE_3D, texture);
        this->setInt(name, id);
    }

    void uninitialize(void)
    {
        UninitializeShaders(shaderProgramObject);
    }
};
