#pragma once

#include <iostream>
#include <vector>
using namespace std;
#include "../../shaders/color/ColorShader.h"

// This includes the global perspectiveProjectionMatrix
#include "../common.h"

class BezierCamera
{
    // random_ is of type Random defined in /helpers/Random.js
public:
    float time = 0.0;
    vector<float> vertices;

    vmath::vec3 position = vmath::vec3(0, 0, 0);

    vmath::vec3 front = vmath::vec3(0.0, 0.0, -1.0);
    vmath::vec3 up = vmath::vec3(0.0, 1.0, 0.0);
    vmath::vec3 right;

    float yaw = -90.0;
    float pitch = 0.0;
    vmath::vec3 worldUp = vmath::vec3(0.0, 1.0, 0.0);

    ///////////////////////////////////// Bezier Points
    // We need to fill these points by calling setBezierPoints
    vector<vector<float>> bezierPoints;
    vector<float> yawPoints;
    vector<float> pitchPoints;

    vector<float> perspectivePoints;

    ColorShader colorShader;

    // Cyilnder
    GLuint vao_line;
    GLuint vbo_line_Position;
    bool handlePerspective = false;
    float fov;
    bool isWater = false;
    const float verticesPoints[30] = {
        0.0f, 0.0f, 50.0f,
        -12.0f, 0.0f, 20.0f,
        20.0f, 0.0f, 10.0f,
        20.0f, 0.0f, 10.0f,
        20.0f, 0.0f, 10.0f,
        20.0f, 0.0f, 10.0f,
        20.0f, 0.0f, 10.0f,
        20.0f, 0.0f, 10.0f,
        20.0f, 0.0f, 10.0f,
        0.0f, 0.0f, 6.0f};

    void initialize(void)
    {
        colorShader.initialize();

        //  vao_line
        glGenVertexArrays(1, &vao_line);
        glBindVertexArray(vao_line);
        {
            // vbo for position
            glGenBuffers(1, &vbo_line_Position); // Create vertex data buffer
            glBindBuffer(GL_ARRAY_BUFFER, vbo_line_Position);
            {
                glBufferData(GL_ARRAY_BUFFER, NULL, NULL, GL_DYNAMIC_DRAW);
                glVertexAttribPointer(MATRIX_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
                glEnableVertexAttribArray(MATRIX_ATTRIBUTE_POSITION);
            }
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            // Color
            {
                glVertexAttrib4f(MATRIX_ATTRIBUTE_COLOR, 1.0f, 1.0f, 1.0f, 1.0f);
            }
        }
        glBindVertexArray(0);
    }

    // // Source: Wikipedia
    // Function to get a particular column of a 2D vector
    vector<float> extractColumn(const vector<vector<float>> &vec2D, int colIndex)
    {
        vector<float> column;

        // Iterate over each row of the 2D vector
        for (const auto &row : vec2D)
        {
            // Check if the row has enough columns
            if (colIndex < row.size())
            {
                // Add the element at the specified column index to the column vector
                column.push_back(row[colIndex]);
            }
        }

        return column;
    }

    float deCasteljau(float t, vector<float> coefs)
    {
        vector<float> beta = coefs;
        int n = beta.size();
        for (int j = 1; j < n; j++)
        {
            for (int k = 0; k < n - j; k++)
            {
                beta[k] = beta[k] * (1 - t) + beta[k + 1] * t;
            }
        }
        return beta[0];
    }

    void updateCameraVectors()
    {
        // Calculating yaw and pitch
        yaw = deCasteljau(time, yawPoints);
        pitch = deCasteljau(time, pitchPoints);
        if (handlePerspective && perspectivePoints.size() > 0)
        {
            fov = deCasteljau(time, perspectivePoints);
            perspectiveProjectionMatrix = vmath::perspective(45.0f + degToRad(fov), (GLfloat)giWindowWidth / (GLfloat)giWindowHeight, 0.1f, 10000000.0f);
        }
        // updating center [ YAW & PITCH ]
        vmath::vec3 front_ = vmath::vec3(
            cos(degToRad(yaw)) * cos(degToRad(pitch)),
            sin(degToRad(pitch)),
            sin(degToRad(yaw)) * cos(degToRad(pitch)));

        front = vmath::normalize(front_);

        // Recalculate right
        right = vmath::normalize(vmath::cross(front, worldUp));

        // Recalculate up
        up = vmath::normalize(vmath::cross(right, front));

        // Updating Position
        // position = calculateBezierCurve(this.bezierPoints, this.time, null, 0);
        position[0] = deCasteljau(time, extractColumn(bezierPoints, 0));
        position[1] = deCasteljau(time, extractColumn(bezierPoints, 1));
        position[2] = deCasteljau(time, extractColumn(bezierPoints, 2));
    }

    void updateInversCameraVectors()
    {
        // Calculating yaw and pitch
        // yaw = deCasteljau(time, yawPoints);
        // pitch = deCasteljau(time, pitchPoints);
        pitch = -pitch;

        // updating center [ YAW & PITCH ]
        vmath::vec3 front_ = vmath::vec3(
            cos(degToRad(yaw)) * cos(degToRad(pitch)),
            sin(degToRad(pitch)),
            sin(degToRad(yaw)) * cos(degToRad(pitch)));

        front = vmath::normalize(front_);

        // Recalculate right
        right = vmath::normalize(vmath::cross(front, worldUp));

        // Recalculate up
        up = vmath::normalize(vmath::cross(right, front));

        // Updating Position
        // position = calculateBezierCurve(this.bezierPoints, this.time, null, 0);
        // position[0] = deCasteljau(time, extractColumn(bezierPoints, 0));
        // position[1] = deCasteljau(time, extractColumn(bezierPoints, 1));
        // position[2] = deCasteljau(time, extractColumn(bezierPoints, 2));

        // float distance = 2.0f * (position[1] - 0.0f );
        // position[1] -= distance;
    }

    // Calculates viewMatrix based on the point returned by the bezier curve for current time.
    vmath::mat4 getViewMatrix(void)
    {
        vmath::vec3 center;
        center = position + front;

        return vmath::lookat(position, center, up);
    }

    vmath::vec3 getCenter(void)
    {
        return (position + front);
    }

    vmath::vec3 getUp()
    {
        return up;
    }
    vmath::vec3 getEye(void)
    {
        return position;
    }

    void invertPitch(void)
    {
        // pitch = -pitch;
        // updateCameraVectors();
        updateInversCameraVectors();
    }

    // Sets the bezier points
    void setBezierPoints(vector<vector<float>> points_, vector<float> yawPoints_, vector<float> pitchPoints_)
    {
        bezierPoints = points_;
        yawPoints = yawPoints_;
        pitchPoints = pitchPoints_;
    }
    void setBezierPoints(vector<vector<float>> points_, vector<float> yawPoints_, vector<float> pitchPoints_, vector<float> perspectivePoints_)
    {
        bezierPoints = points_;
        yawPoints = yawPoints_;
        pitchPoints = pitchPoints_;
        perspectivePoints = perspectivePoints_;
    }

    // Gets individual camera position
    vmath::vec3 getCameraPosition(void)
    {
        return position;
    }

    // Displays a trailing line tracing the bezier curve
    void displayBezierCurve()
    {
        // Drawing camera trailing light
        // pushMatrix(modelMatrix);
        {

            glUseProgram(colorShader.shaderProgramObject);
            // Uniforms
            glUniformMatrix4fv(colorShader.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
            glUniformMatrix4fv(colorShader.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
            glUniformMatrix4fv(colorShader.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

            glBindVertexArray(vao_line);
            glVertexAttrib4f(MATRIX_ATTRIBUTE_COLOR, 1.0, 1.0, 1.0, 1.0);

            vector<float> tempVertices;
            // Building bezier vertices
            for (float i = 0; i <= 1.0; i += 0.01)
            {
                tempVertices.insert(tempVertices.end(),
                                    {deCasteljau(i, extractColumn(bezierPoints, 0)),
                                     deCasteljau(i, extractColumn(bezierPoints, 1)),
                                     deCasteljau(i, extractColumn(bezierPoints, 2))});
            }

            glBindBuffer(GL_ARRAY_BUFFER, vbo_line_Position);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * tempVertices.size(), tempVertices.data(), GL_DYNAMIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, NULL);

            glDrawArrays(GL_LINE_STRIP, 0, tempVertices.size() / 3);
            glBindVertexArray(NULL);

            // Main points
            // Convert the 2D vector to a 1D vector
            vector<float> bezierPoints1D;
            for (const auto &row : bezierPoints)
            {
                bezierPoints1D.insert(bezierPoints1D.end(), row.begin(), row.end());
            }
            // set size to 5 for another group of points
            glPointSize(5);
            glBindVertexArray(vao_line);
            glVertexAttrib4f(MATRIX_ATTRIBUTE_COLOR, 1.0, 0.0, 0.0, 1.0);
            glBindBuffer(GL_ARRAY_BUFFER, vbo_line_Position);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * bezierPoints1D.size(), bezierPoints1D.data(), GL_DYNAMIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, NULL);

            glDrawArrays(GL_POINTS, 0, bezierPoints1D.size() / 3);
            glBindVertexArray(NULL);
        }
        // modelMatrix = popMatrix();
    }

    // This should be called from the main index.js as this sphere might be drawn multiple times
    void update()
    {
        // Updating all the camera vectors
        if (!isWater)
            updateCameraVectors();
        else
        {
            yaw = deCasteljau(time, yawPoints);
            pitch = deCasteljau(time, pitchPoints);
            position[0] = deCasteljau(time, extractColumn(bezierPoints, 0));
            position[1] = deCasteljau(time, extractColumn(bezierPoints, 1));
            position[2] = deCasteljau(time, extractColumn(bezierPoints, 2));
        }
    }

    void uninitialize()
    {

        if (vbo_line_Position)
        {
            glDeleteBuffers(1, &vbo_line_Position);
            vbo_line_Position = 0;
        }

        if (vao_line)
        {
            glDeleteVertexArrays(1, &vao_line);
            vao_line = 0;
        }

        // Uninitializing shaders
        colorShader.uninitialize();
    }
};
