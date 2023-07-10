#pragma once

#include "../../utils/common.h"
#include "../../shaders/model/Model_Shader.h"
#include "../../utils/camera/BezierCamera.h"
#include "../../utils/camera/Camera.h"

#define AAG_RIGHT_START_TIME 176.2f

void setGlobalBezierCamera(BezierCamera *bezierCamera);

extern Camera camera;
extern BezierCamera *globalBezierCamera;

class Opening
{

public:
    // member variables
    BezierCamera *aagCam;

    BOOL isInitialized = FALSE;
    BOOL hideFire = FALSE;

    // Barsaat Texture
    GLuint VAO_cube;
    GLuint VBO_cube_position;
    GLuint VBO_cube_texcoord;
    GLuint texture_BarsaatPoster;

    // member functions
    Opening()
    {
        aagCam = new BezierCamera();
    }

    BOOL initialize()
    {
        std::vector<std::vector<float>> bezierPointsAag = {

        };

        // YAW GLOBAL
        std::vector<float> yawAag = {

        };

        // PITCH GLOBAL
        std::vector<float> pitchAag = {

        };

        // FOV GLOBAL
        std::vector<float> fovAag = {

        };
        aagCam->initialize();
        aagCam->handlePerspective = true;
        aagCam->setBezierPoints(bezierPointsAag, yawAag, pitchAag, fovAag);
        aagCam->update();
        setGlobalBezierCamera(aagCam);

        isInitialized = TRUE;

        return (TRUE);
    }

    void display()
    {
    }

    void update()
    {
    }

    void uninitialize()
    {
        if (isInitialized)
        {

            if (aagCam)
            {
                aagCam->uninitialize();
                delete aagCam;
                aagCam = NULL;
            }
        }
    }
};
