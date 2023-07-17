#pragma once
#include "../../utils/OpenGL/GLHeadersAndMacros.h"
#include "../../utils/OpenGL/GLShaders.h"
#include "mcubes.h"
#include "MetaballHelper.h"

class Metaball
{
public:
    // Variables

    // Metaball variables
    mcubes *mc;
    mcubes_metaballs *mb;

    BOOL initialize(void)
    {
        // Metaballs
        mb = new mcubes_metaballs();
        mc = new mcubes(-10, 10, 10, 10, -10, -10, 0.5, -0.5, -0.5);
        mc->setMetaBalls(mb, 0.3f);

        return TRUE;
    }

    void update()
    {
    }

    void display()
    {
        pushMatrix(modelMatrix);
        {

            modelMatrix = modelMatrix * translate(0.0f, 0.0f, -10.0f);
            // code:
            glUseProgram(commonShaders->textureShader->shaderProgramObject);
            glUniformMatrix4fv(commonShaders->textureShader->modelMatrixUniform, 1, GL_FALSE, modelMatrix);
            glUniformMatrix4fv(commonShaders->textureShader->viewMatrixUniform, 1, GL_FALSE, viewMatrix);
            glUniformMatrix4fv(commonShaders->textureShader->projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glActiveTexture(GL_TEXTURE0);
            glUniform1i(commonShaders->textureShader->textureSamplerUniform, 0);
            glUniform1f(commonShaders->textureShader->alphaValueUniform, 1.0f);

        
            double static movement = 0.0;
            double movement_factor = 0.01f;

            movement += (0.01);

            /* flytt de faktiske ballene */
            // mb->move_ball(0, cos(movement) * movement_factor, sin(movement) * movement_factor, cos(movement * 2) * movement_factor);
            // mb->move_ball(0, objX, objY, objZ);
            mb->move_ball(1, sin(movement) * movement_factor, cos(movement) * movement_factor, sin(movement * 2) * movement_factor);

            mc->computeMetaBalls();
            mc->draw();

            glBindTexture(GL_TEXTURE_2D, 0);
            glDisable(GL_BLEND);
            glUseProgram(0);
        }
        modelMatrix = popMatrix();
    }

    void uninitialize(void)
    {
    }
};
