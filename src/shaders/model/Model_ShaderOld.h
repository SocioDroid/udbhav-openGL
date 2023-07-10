#pragma once

// #include "../03_UtilityFiles/GLHeadersAndMacros.h"
// #include "../03_UtilityFiles/GLShaders.h"
// #include "../../utils/model/mesh.h"
#include "../../utils/model_using_assim/assim.h"
#include "../../utils/common.h"

class ModelShader
{

public:
    // Variables
    GLuint shaderProgramObject;

    GLuint projectMatrixUniform; // model View Projection
    GLuint viewMatrixUniform;    // model View Projection
    GLuint modelMatrixUniform;   // model View Projection

    GLuint vao_Model;
    GLuint vbo_Model_Position;
    GLuint vbo_Model_Normal;
    GLuint vbo_Model_Texcoord;
    GLuint vbo_Matrices;

    GLuint laUniform_PF;           // light Ambiant
    GLuint ldUniform_PF;           // light Diffuse
    GLuint lsUniform_PF;           // light Spec
    GLuint lighPositionUniform_PF; // light Position

    GLuint kaUniform_PF; // material amb
    GLuint kdUniform_PF; // mat diff
    GLuint ksUniform_PF; // mat specular
    GLuint materialShininessUniform_PF;

    GLuint lightingEnabledUniform_PF;

    GLuint textureSamplerUniform;
    GLuint normalSamplerUniform;
    GLuint matrixUniform;
    GLuint clipPlaneUniform;

    GLuint texture;
    GLuint textureNormal;
    BOOL bLight = TRUE;

    GLfloat lightAmbiant[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    GLfloat lightDiffuse[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat lightSpecular[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat lightPositions[4] = {0.0f, 100.0f, 100.0f, 1.0f};

    GLfloat materialAmbiant[4] = {0.2f, 0.2f, 0.2f, 1.0f};
    GLfloat meterialDeffuse[4] = {0.5f, 0.5f, 0.5f, 1.0f};
    GLfloat materialSpecular[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat materialShineeness = 20.0f;

    // mesh_data_T *mesh_data = nullptr;

    // STL_MODELLOADING model;

    Model *ourModel;

    BOOL initialize_ModelShaderObject(const char *path, const char *textureFileName)
    {
        // vertex Shader
        GLuint vertexShaderObject = CreateAndCompileShaderObjects(".\\src\\shaders\\model\\model.vs", VERTEX);

        // fragment Shader
        GLuint fragmentShaderObject = CreateAndCompileShaderObjects(".\\src\\shaders\\model\\model.fs", FRAGMENT);

        shaderProgramObject = glCreateProgram();
        glAttachShader(shaderProgramObject, vertexShaderObject);
        glAttachShader(shaderProgramObject, fragmentShaderObject);

        // prelinked binding
        glBindAttribLocation(shaderProgramObject, MATRIX_ATTRIBUTE_POSITION, "a_position");
        glBindAttribLocation(shaderProgramObject, MATRIX_ATTRIBUTE_NORMAL, "a_normal");
        glBindAttribLocation(shaderProgramObject, MATRIX_ATTRIBUTE_TEXTURE0, "a_texcoord");
        // link
        BOOL bShaderLinkStatus = LinkShaderProgramObject(shaderProgramObject);

        if (bShaderLinkStatus == FALSE)
            return FALSE;

        // post link - getting
        projectMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_projectionMatrix");
        viewMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_viewMatrix");
        modelMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_modelMatrix");

        laUniform_PF = glGetUniformLocation(shaderProgramObject, "u_la");
        ldUniform_PF = glGetUniformLocation(shaderProgramObject, "u_ld");
        lsUniform_PF = glGetUniformLocation(shaderProgramObject, "u_ls");
        lighPositionUniform_PF = glGetUniformLocation(shaderProgramObject, "u_lightPosition");

        kaUniform_PF = glGetUniformLocation(shaderProgramObject, "u_ka");
        kdUniform_PF = glGetUniformLocation(shaderProgramObject, "u_kd");
        ksUniform_PF = glGetUniformLocation(shaderProgramObject, "u_ks");

        materialShininessUniform_PF = glGetUniformLocation(shaderProgramObject, "u_materialShininnes");

        lightingEnabledUniform_PF = glGetUniformLocation(shaderProgramObject, "u_lightingEnabled");

        textureSamplerUniform = glGetUniformLocation(shaderProgramObject, "u_textureSampler");
        normalSamplerUniform = glGetUniformLocation(shaderProgramObject, "u_normalMap");
        matrixUniform = glGetUniformLocation(shaderProgramObject, "offsets");
        clipPlaneUniform = glGetUniformLocation(shaderProgramObject, "u_plane");

        // mesh_data = load_mesh(path);
        PrintLog("model.loadModel(path)\n");
        // model.loadModel(path);
        ourModel = new Model(path);
        // ourModel->meshes.at(0).vertices.size();
        PrintLog("model.loadModel(path)\n");
        // // vao_Model and VBO related code
        // // vao_Model
        // glGenVertexArrays(1, &vao_Model);
        // glBindVertexArray(vao_Model);

        // // VBO for position
        // glGenBuffers(1, &vbo_Model_Position);
        // glBindBuffer(GL_ARRAY_BUFFER, vbo_Model_Position);
        // // glBufferData(GL_ARRAY_BUFFER, mesh_data->gp_vertex_filter->size * sizeof(float), mesh_data->gp_vertex_filter->p_arr, GL_STATIC_DRAW);
        // glBufferData(GL_ARRAY_BUFFER, model.vertecesAfterFilter.size() * sizeof(float), &model.vertecesAfterFilter[0], GL_STATIC_DRAW);
        // glVertexAttribPointer(MATRIX_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        // glEnableVertexAttribArray(MATRIX_ATTRIBUTE_POSITION);
        // glBindBuffer(GL_ARRAY_BUFFER, 0);

        // // VBO for Texcoord
        // glGenBuffers(1, &vbo_Model_Texcoord);
        // glBindBuffer(GL_ARRAY_BUFFER, vbo_Model_Texcoord);
        // // glBufferData(GL_ARRAY_BUFFER, mesh_data->gp_texture_filter->size * sizeof(float), mesh_data->gp_texture_filter->p_arr, GL_STATIC_DRAW);
        // glBufferData(GL_ARRAY_BUFFER, model.texCoordAfterFilter.size() * sizeof(float), &model.texCoordAfterFilter[0], GL_STATIC_DRAW);
        // glVertexAttribPointer(MATRIX_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
        // glEnableVertexAttribArray(MATRIX_ATTRIBUTE_TEXTURE0);
        // glBindBuffer(GL_ARRAY_BUFFER, 0);

        // // VBO for Normal
        // glGenBuffers(1, &vbo_Model_Normal);
        // glBindBuffer(GL_ARRAY_BUFFER, vbo_Model_Normal);
        // // glBufferData(GL_ARRAY_BUFFER, mesh_data->gp_normal_filter->size * sizeof(float), mesh_data->gp_normal_filter->p_arr, GL_STATIC_DRAW);
        // glBufferData(GL_ARRAY_BUFFER, model.normalAfterFilter.size() * sizeof(float), &model.normalAfterFilter[0], GL_STATIC_DRAW);
        // glVertexAttribPointer(MATRIX_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        // glEnableVertexAttribArray(MATRIX_ATTRIBUTE_NORMAL);
        // glBindBuffer(GL_ARRAY_BUFFER, 0);

        // // VBO for Normal
        // glGenBuffers(1, &vbo_Matrices);
        // glBindBuffer(GL_ARRAY_BUFFER, vbo_Matrices);
        // glBufferData(GL_ARRAY_BUFFER, 9.0f * sizeof(float), NULL, GL_DYNAMIC_DRAW);
        // glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        // glEnableVertexAttribArray(4);
        // glBindBuffer(GL_ARRAY_BUFFER, 0);

        // glBindVertexArray(0);

        if (LoadPNGImage(&texture, textureFileName) == FALSE)
        {
            PrintLog("LoadPNGImage Failed...\n");
            // return FALSE;
        }

        if (LoadPNGImage(&textureNormal, "./src/04_Assets/03_3DModels/Fern/NormalMap.png") == FALSE)
        {
            PrintLog("LoadPNGImage Failed...\n");
            // return FALSE;
        }

        return TRUE;
    }

    void update_ModelShaderObjectUniforms(void)
    {
    }

    void render_Model(void)
    {
        // GLfloat eqn[] = {0.0f, -1.0f, 0.0f, 0.0f};
        // if (isUP == TRUE)
        // {
        //     eqn[1] = 1.0f;
        //     // eqn[3] = -8.5f;
        //     eqn[3] = 0.0f;
        // }
        // else
        // {
        //     eqn[1] = 1.0f;
        //     // eqn[3] = 8.5f;
        //     eqn[3] = 0.0f;
        // }

        float matrix[3] = {objX, objY, objZ};

        // lightPositions[0] = eye[0];
        // lightPositions[1] = eye[1];
        // lightPositions[2] = eye[2];

        // Use The Shader Program Object
        glUseProgram(shaderProgramObject);

        // Transformations
        glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
        glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);
        glUniformMatrix4fv(projectMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

        glUniform3fv(matrixUniform, 1, matrix);

        // glUniform4fv(clipPlaneUniform, 1, eqn);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(textureSamplerUniform, 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textureNormal);
        glUniform1i(textureSamplerUniform, 1);

        // Sending Light Related Uniforms
        if (bLight == TRUE)
        {

            glUniform1i(lightingEnabledUniform_PF, 1);

            glUniform3fv(laUniform_PF, 1, lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
            glUniform3fv(ldUniform_PF, 1, lightDiffuse);
            glUniform3fv(lsUniform_PF, 1, lightSpecular);
            glUniform4fv(lighPositionUniform_PF, 1, lightPositions);

            glUniform3fv(kaUniform_PF, 1, materialAmbiant);
            glUniform3fv(kdUniform_PF, 1, meterialDeffuse);
            glUniform3fv(ksUniform_PF, 1, materialSpecular);
            glUniform1f(materialShininessUniform_PF, materialShineeness);
        }
        else
        {
            glUniform1i(lightingEnabledUniform_PF, 0);
        }
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        // glBindVertexArray(vao_Model);
        // glDrawArrays(GL_TRIANGLES, 0, mesh_data->gp_vertex_filter->size);
        // glDrawArraysInstanced(GL_TRIANGLES, 0, model.vertecesAfterFilter.size(), 1);
        // glBindVertexArray(0);
        ourModel->Render(shaderProgramObject);
        // Unuse The Shader Program Object
        glUseProgram(0);
    }

    void render_Models(const int numberOfInstances, float *instancesPositionArray, BOOL isUP)
    {
        GLfloat eqn[] = {0.0f, -1.0f, 0.0f, 0.0f};
        if (isUP == TRUE)
        {
            eqn[1] = 1.0f;
            eqn[3] = 0.0f;
        }
        else
        {
            eqn[1] = -1.0f;
            eqn[3] = 0.0f;
        }

        // float matrix[6] = {-2.0, 0.0, -2.0,
        //                    2.0, 0.0, 0.0};

        // lightPositions[0] = eye[0];
        // lightPositions[1] = eye[1];
        // lightPositions[2] = eye[2];

        // Use The Shader Program Object
        glUseProgram(shaderProgramObject);

        // Transformations
        glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
        glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);
        glUniformMatrix4fv(projectMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(textureSamplerUniform, 0);

        glUniform3fv(matrixUniform, numberOfInstances, instancesPositionArray);

        glUniform4fv(clipPlaneUniform, 1, eqn);

        // Sending Light Related Uniforms
        if (bLight == TRUE)
        {
            glUniform1i(lightingEnabledUniform_PF, 1);

            glUniform3fv(laUniform_PF, 1, lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
            glUniform3fv(ldUniform_PF, 1, lightDiffuse);
            glUniform3fv(lsUniform_PF, 1, lightSpecular);
            glUniform4fv(lighPositionUniform_PF, 1, lightPositions);

            glUniform3fv(kaUniform_PF, 1, materialAmbiant);
            glUniform3fv(kdUniform_PF, 1, meterialDeffuse);
            glUniform3fv(ksUniform_PF, 1, materialSpecular);
            glUniform1f(materialShininessUniform_PF, materialShineeness);
        }
        else
        {
            glUniform1i(lightingEnabledUniform_PF, 0);
        }

        // glBindVertexArray(vao_Model);

        // glBindBuffer(GL_ARRAY_BUFFER, vbo_Matrices);
        // glBufferData(GL_ARRAY_BUFFER, 9.0f * 16.0f * sizeof(float), matrices, GL_DYNAMIC_DRAW);
        // glBindBuffer(GL_ARRAY_BUFFER, 0);

        // glDrawArrays(GL_TRIANGLES, 0, mesh_data->gp_vertex_filter->size);
        // glDrawArraysInstanced(GL_TRIANGLES, 0, model.vertecesAfterFilter.size(), numberOfInstances);

        // glBindVertexArray(0);

        // Unuse The Shader Program Object
        glUseProgram(0);
    }

    void render_Models1(const int numberOfInstances, mat4 matrices)
    {

        // float matrix[6] = {-2.0, 0.0, -2.0,
        //                    2.0, 0.0, 0.0};

        lightPositions[0] = eye[0];
        lightPositions[1] = eye[1];
        lightPositions[2] = eye[2];

        // Use The Shader Program Object
        glUseProgram(shaderProgramObject);

        // Transformations
        glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
        glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);
        glUniformMatrix4fv(projectMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(textureSamplerUniform, 0);

        glUniformMatrix4fv(matrixUniform, numberOfInstances, GL_FALSE, matrices[0]);

        // Sending Light Related Uniforms
        if (bLight == TRUE)
        {
            glUniform1i(lightingEnabledUniform_PF, 1);

            glUniform3fv(laUniform_PF, 1, lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
            glUniform3fv(ldUniform_PF, 1, lightDiffuse);
            glUniform3fv(lsUniform_PF, 1, lightSpecular);
            glUniform4fv(lighPositionUniform_PF, 1, lightPositions);

            glUniform3fv(kaUniform_PF, 1, materialAmbiant);
            glUniform3fv(kdUniform_PF, 1, meterialDeffuse);
            glUniform3fv(ksUniform_PF, 1, materialSpecular);
            glUniform1f(materialShininessUniform_PF, materialShineeness);
        }
        else
        {
            glUniform1i(lightingEnabledUniform_PF, 0);
        }

        // glBindVertexArray(vao_Model);

        // glBindBuffer(GL_ARRAY_BUFFER, vbo_Matrices);
        // glBufferData(GL_ARRAY_BUFFER, 9.0f * 16.0f * sizeof(float), matrices, GL_DYNAMIC_DRAW);
        // glBindBuffer(GL_ARRAY_BUFFER, 0);

        // glDrawArrays(GL_TRIANGLES, 0, mesh_data->gp_vertex_filter->size);
        // glDrawArraysInstanced(GL_TRIANGLES, 0, model.vertecesAfterFilter.size(), numberOfInstances);

        // glBindVertexArray(0);

        // Unuse The Shader Program Object
        glUseProgram(0);
    }

    void uninitialize_ModelShaderObject(void)
    {
        if (ourModel)
        {
            delete (ourModel);
            ourModel = NULL;
        }
        // if (mesh_data)
        // {
        //     free_mesh_data(&mesh_data);
        // }

        if (texture)
        {
            glDeleteTextures(1, &texture);
            texture = 0;
        }

        if (shaderProgramObject)
        {
            UninitializeShaders(shaderProgramObject);
        }
    }
};