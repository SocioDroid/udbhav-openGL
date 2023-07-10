#pragma once
#include "../../utils/OpenGL/GLHeadersAndMacros.h"
#include "../../utils/OpenGL/GLShaders.h"

#include "../../effects/cloth/Cloth.h"

#define SHADER_OBJECT_ERROR -100
#define SHADER_PROGRAM_OBJECT_ERROR -101

class ClothShader
{
public:
    // Variables
    // ########## MASS - SPRING SHADER UNIFORMS AND VARIABLES ##########
    GLuint shaderProgramObject_MassSpring;

    GLuint modelMatrixUniform_MassSpring;
    GLuint viewMatrixUniform_MassSpring;
    GLuint projectionMatrixUniform_MassSpring;

    GLuint texPositionMassSamplerUniform_MassSpring;
    GLuint previousTexPositionMassSamplerUniform_MassSpring;

    GLuint inverseClothSizeUniform_MassSpring;
    GLuint stepUniform_MassSpring;
    GLuint texSizeXUniform_MassSpring;
    GLuint texSizeYUniform_MassSpring;

    GLuint deltaTimeUniform_MassSpring;

    GLuint ksStructUniform_MassSpring;
    GLuint kdStructUniform_MassSpring;
    GLuint ksShearUniform_MassSpring;
    GLuint kdShearUniform_MassSpring;
    GLuint ksBendUniform_MassSpring;
    GLuint kdBendUniform_MassSpring;

    GLuint defaultDampingUniform_MassSpring;

    GLuint gravityUniform_MassSpring;

    GLuint backWallCollisionDetectionEnableUniform_MassSpring;

    GLuint dupattaFallUniform_MassSpring;

    // ########## PARTICLE SHADER UNIFORMS AND VARIABLES ##########
    GLuint shaderProgramObject_Particle;

    GLuint modelMatrixUniform_Particle;
    GLuint viewMatrixUniform_Particle;
    GLuint projectionMatrixUniform_Particle;

    GLuint pointSizeUniform_Particle;
    GLuint selectedIndexUniform_Particle;
    GLuint colorUniform_Particle;

    // ########## RENDER SHADER UNIFORMS AND VARIABLES ##########
    GLuint shaderProgramObject_Render;

    GLuint modelMatrixUniform_Render;
    GLuint viewMatrixUniform_Render;
    GLuint projectionMatrixUniform_Render;

    GLuint textureTilingUniform_Render;

    GLuint clothTextureSamplerUniform_Render;

    GLuint clothAlphaUniform_Render;

    unsigned int clothInScene;

    ClothShader(unsigned int scene)
    {
        clothInScene = scene;
    }

    // Member Functions
    BOOL initialize()
    {
        // variable declarations
        int iRetVal = 0;

        // code
        
        // ****** MASS-SPRING SHADER ******
        iRetVal = initializeClothMassSpringShader();
        if (iRetVal != 0)
        {
            PrintLog("initializeClothMassSpringShader() Failed.\n");
            return(FALSE);
        }
        else
            PrintLog("initializeClothMassSpringShader() Succeeded.\n");

        // ****** PARTICLE SHADERS ******
        iRetVal = initializeClothParticleShaders();
        if (iRetVal != 0)
        {
            PrintLog("initializeClothParticleShaders() Failed.\n");
            return(FALSE);
        }
        else
            PrintLog("initializeClothParticleShaders() Succeeded.\n");

        // ****** RENDER SHADERS ******
        iRetVal = initializeClothRenderShaders();
        if (iRetVal != 0)
        {
            PrintLog("initializeClothRenderShaders() Failed.\n");
            return(FALSE);
        }
        else
            PrintLog("initializeClothRenderShaders() Succeeded.\n");

        return(TRUE);
    }

    int initializeClothMassSpringShader(void)
    {
        // ****** SHADERS ******
        GLuint vertexShaderObject = 0;

        if(clothInScene == CLOTH_IN_AAG_SCENE)
            vertexShaderObject = CreateAndCompileShaderObjects(".\\src\\shaders\\cloth\\ClothMassSpring_Aag_Curtain.vs", VERTEX);

        else if(clothInScene == CLOTH_IN_BARSAAT_SCENE_DUPATTA)
            vertexShaderObject = CreateAndCompileShaderObjects(".\\src\\shaders\\cloth\\ClothMassSpring_Barsaat_Dupatta.vs", VERTEX);

        else if(clothInScene == CLOTH_IN_BARSAAT_SCENE_CURTAIN)
            vertexShaderObject = CreateAndCompileShaderObjects(".\\src\\shaders\\cloth\\ClothMassSpring_Barsaat_Curtain.vs", VERTEX);

        else if(clothInScene == CLOTH_IN_JIS_DESH_MEIN_SCENE)
            vertexShaderObject = CreateAndCompileShaderObjects(".\\src\\shaders\\cloth\\ClothMassSpring_JisDeshMein_Dupatta.vs", VERTEX);

        else if(clothInScene == CLOTH_IN_SANGAM_SCENE)
            vertexShaderObject = CreateAndCompileShaderObjects(".\\src\\shaders\\cloth\\ClothMassSpring_Sangam_Curtain.vs", VERTEX);

        if (vertexShaderObject == 0)
        {
            PrintLog("CreateAndCompileShaderObjects() Failed For \"ClothMassSpring.vs\" Shader File.\n");
            return(SHADER_OBJECT_ERROR);
        }

        shaderProgramObject_MassSpring = glCreateProgram();

        // Step 2
        glAttachShader(shaderProgramObject_MassSpring, vertexShaderObject);

        // Pre-Linking Binding Of Vertex Attribute Locations
        glBindAttribLocation(shaderProgramObject_MassSpring, MATRIX_CLOTH_ATTRIBUTE_POSITION_MASS, "a_positionMass");
        glBindAttribLocation(shaderProgramObject_MassSpring, MATRIX_CLOTH_ATTRIBUTE_PREVIOUS_POSITION, "a_previousPosition");

        if (LinkShaderProgramObject(shaderProgramObject_MassSpring) == FALSE)
        {
            PrintLog("LinkShaderProgramObject() Failed For \"Cloth Mass-Spring\" Shaders.\n");
            return(SHADER_PROGRAM_OBJECT_ERROR);
        }

        // Post-Linking Getting Of Uniform Locations
        modelMatrixUniform_MassSpring = glGetUniformLocation(shaderProgramObject_MassSpring, "u_modelMatrix");
        viewMatrixUniform_MassSpring = glGetUniformLocation(shaderProgramObject_MassSpring, "u_viewMatrix");
        projectionMatrixUniform_MassSpring = glGetUniformLocation(shaderProgramObject_MassSpring, "u_projectionMatrix");

        texPositionMassSamplerUniform_MassSpring = glGetUniformLocation(shaderProgramObject_MassSpring, "u_texPositionMass");
        previousTexPositionMassSamplerUniform_MassSpring = glGetUniformLocation(shaderProgramObject_MassSpring, "u_previousTexPositionMass");

        inverseClothSizeUniform_MassSpring = glGetUniformLocation(shaderProgramObject_MassSpring, "u_inverseClothSize");
        stepUniform_MassSpring = glGetUniformLocation(shaderProgramObject_MassSpring, "u_step");
        texSizeXUniform_MassSpring = glGetUniformLocation(shaderProgramObject_MassSpring, "u_texSizeX");
        texSizeYUniform_MassSpring = glGetUniformLocation(shaderProgramObject_MassSpring, "u_texSizeY");

        deltaTimeUniform_MassSpring = glGetUniformLocation(shaderProgramObject_MassSpring, "u_deltaTime");

        ksStructUniform_MassSpring = glGetUniformLocation(shaderProgramObject_MassSpring, "u_ksStruct");
        kdStructUniform_MassSpring = glGetUniformLocation(shaderProgramObject_MassSpring, "u_kdStruct");
        ksShearUniform_MassSpring = glGetUniformLocation(shaderProgramObject_MassSpring, "u_ksShear");
        kdShearUniform_MassSpring = glGetUniformLocation(shaderProgramObject_MassSpring, "u_kdShear");
        ksBendUniform_MassSpring = glGetUniformLocation(shaderProgramObject_MassSpring, "u_ksBend");
        kdBendUniform_MassSpring = glGetUniformLocation(shaderProgramObject_MassSpring, "u_kdBend");
        
        defaultDampingUniform_MassSpring = glGetUniformLocation(shaderProgramObject_MassSpring, "u_defaultDamping");

        gravityUniform_MassSpring = glGetUniformLocation(shaderProgramObject_MassSpring, "u_gravity");

        backWallCollisionDetectionEnableUniform_MassSpring = glGetUniformLocation(shaderProgramObject_MassSpring, "u_backWallCollisionDetectionEnable");

        if(clothInScene == CLOTH_IN_BARSAAT_SCENE_DUPATTA)
            dupattaFallUniform_MassSpring = glGetUniformLocation(shaderProgramObject_MassSpring, "u_dupattaFall");
        
        return(0);
    }

    int initializeClothParticleShaders(void)
    {
        // ****** SHADERS ******
        GLuint vertexShaderObject = CreateAndCompileShaderObjects(".\\src\\shaders\\cloth\\ClothParticle.vs", VERTEX);
        if (vertexShaderObject == 0)
        {
            PrintLog("CreateAndCompileShaderObjects() Failed For \"ClothParticle.vs\" Shader File.\n");
            return(SHADER_OBJECT_ERROR);
        }

        GLuint fragmentShaderObject = CreateAndCompileShaderObjects(".\\src\\shaders\\cloth\\ClothParticle.fs", FRAGMENT);
        if (fragmentShaderObject == 0)
        {
            PrintLog("CreateAndCompileShaderObjects() Failed For \"ClothParticle.fs\" Shader File.\n");
            return(SHADER_OBJECT_ERROR);
        }

        shaderProgramObject_Particle = glCreateProgram();

        // Step 2
        glAttachShader(shaderProgramObject_Particle, vertexShaderObject);
        glAttachShader(shaderProgramObject_Particle, fragmentShaderObject);

        // Pre-Linking Binding Of Vertex Attribute Locations
        glBindAttribLocation(shaderProgramObject_Particle, MATRIX_CLOTH_ATTRIBUTE_POSITION_MASS, "a_positionMass");

        if (LinkShaderProgramObject(shaderProgramObject_Particle) == FALSE)
        {
            PrintLog("LinkShaderProgramObject() Failed For \"Cloth Particle\" Shaders.\n");
            return(SHADER_PROGRAM_OBJECT_ERROR);
        }

        // Post-Linking Getting Of Uniform Locations
        modelMatrixUniform_Particle = glGetUniformLocation(shaderProgramObject_Particle, "u_modelMatrix");
        viewMatrixUniform_Particle = glGetUniformLocation(shaderProgramObject_Particle, "u_viewMatrix");
        projectionMatrixUniform_Particle = glGetUniformLocation(shaderProgramObject_Particle, "u_projectionMatrix");

        pointSizeUniform_Particle = glGetUniformLocation(shaderProgramObject_Particle, "u_pointSize");
        selectedIndexUniform_Particle = glGetUniformLocation(shaderProgramObject_Particle, "u_selectedIndex");
        colorUniform_Particle = glGetUniformLocation(shaderProgramObject_Particle, "u_color");

        return(0);
    }

    int initializeClothRenderShaders(void)
    {
        // ****** SHADERS ******
        GLuint vertexShaderObject = CreateAndCompileShaderObjects(".\\src\\shaders\\cloth\\ClothRender.vs", VERTEX);
        if (vertexShaderObject == 0)
        {
            PrintLog("CreateAndCompileShaderObjects() Failed For \"ClothRender.vs\" Shader File.\n");
            return(SHADER_OBJECT_ERROR);
        }

        GLuint fragmentShaderObject = CreateAndCompileShaderObjects(".\\src\\shaders\\cloth\\ClothRender.fs", FRAGMENT);
        if (fragmentShaderObject == 0)
        {
            PrintLog("CreateAndCompileShaderObjects() Failed For \"ClothRender.fs\" Shader File.\n");
            return(SHADER_OBJECT_ERROR);
        }

        shaderProgramObject_Render = glCreateProgram();

        // Step 2
        glAttachShader(shaderProgramObject_Render, vertexShaderObject);
        glAttachShader(shaderProgramObject_Render, fragmentShaderObject);

        // Pre-Linking Binding Of Vertex Attribute Locations
        glBindAttribLocation(shaderProgramObject_Render, MATRIX_CLOTH_ATTRIBUTE_POSITION_MASS, "a_positionMass");

        if (LinkShaderProgramObject(shaderProgramObject_Render) == FALSE)
        {
            PrintLog("LinkShaderProgramObject() Failed For \"Cloth Render\" Shaders.\n");
            return(SHADER_PROGRAM_OBJECT_ERROR);
        }

        // Post-Linking Getting Of Uniform Locations
        modelMatrixUniform_Render = glGetUniformLocation(shaderProgramObject_Render, "u_modelMatrix");
        viewMatrixUniform_Render = glGetUniformLocation(shaderProgramObject_Render, "u_viewMatrix");
        projectionMatrixUniform_Render = glGetUniformLocation(shaderProgramObject_Render, "u_projectionMatrix");

    	textureTilingUniform_Render = glGetUniformLocation(shaderProgramObject_Render, "u_textureTiling");

        clothTextureSamplerUniform_Render = glGetUniformLocation(shaderProgramObject_Render, "u_clothTextureSampler");

        clothAlphaUniform_Render = glGetUniformLocation(shaderProgramObject_Render, "u_alpha");
        
        return(0);
    }       

    void uninitialize(void)
    {
        UninitializeShaders(shaderProgramObject_Render);
        UninitializeShaders(shaderProgramObject_Particle);
        UninitializeShaders(shaderProgramObject_MassSpring);
    }
};

