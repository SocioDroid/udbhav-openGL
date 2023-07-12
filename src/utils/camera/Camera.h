#pragma once

#include <Windows.h>
// This includes the global perspectiveProjectionMatrix
#include "../common.h"

using namespace vmath;

const float YAW = -90.0;
const float PITCH = 0.0;
const float SPEED = 3.2;
const float SENSITIVITY = 0.4;
const float ZOOM = 45.0;

class Camera
{
public:
    float width;
    float height;

    vmath::vec3 position = vmath::vec3(0.0, 20.0, 5.0);
    vmath::vec3 front = vmath::vec3(0.0, 0.0, -1.0);
    vmath::vec3 up = vmath::vec3(0.0, 1.0, 0.0);
    vmath::vec3 right;
    vmath::vec3 worldUp = up;

    // euler angles
    float yaw = YAW;
    float pitch = PITCH;
    // Camera options
    float movementSpeed = SPEED;
    float mouseSensitivity = SENSITIVITY;
    float zoom = ZOOM;

    float lastX = 0.0;
    float lastY = 0.0;
    BOOL firstMouse = TRUE;

    Camera() {}

    Camera(float _width, float _height, float *_position)
    {
        width = _width;
        height = _height;

        // Camera attributes
        position = vmath::vec3(_position[0], _position[1], _position[2]);

        updateCameraVectors();
    }

    vmath::mat4 getViewMatrix(void)
    {
        vmath::mat4 tempViewMatrix = vmath::mat4::identity();
        vmath::vec3 center;
        center = position + front;

        tempViewMatrix = vmath::lookat(position, center, up);

        return tempViewMatrix;
    }

    vmath::vec3 getEye(void)
    {
        return position;
    }

    vmath::vec3 getCenter(void)
    {
        return (position + front);
    }

    vmath::vec3 getUp()
    {
        return up;
    }

    void updateCameraVectors(void)
    {

        vmath::vec3 front_ = vmath::vec3(
            cos(degToRad(yaw)) * cos(degToRad(pitch)),
            sin(degToRad(pitch)),
            sin(degToRad(yaw)) * cos(degToRad(pitch)));

        front = vmath::normalize(front_);

        right = vmath::normalize(vmath::cross(front, worldUp));
        up = vmath::normalize(vmath::cross(right, front));
    }

    void updateInversCameraVectors()
    {
        pitch = -pitch;
        vmath::vec3 front_ = vmath::vec3(
            cos(degToRad(yaw)) * cos(degToRad(pitch)),
            sin(degToRad(pitch)),
            sin(degToRad(yaw)) * cos(degToRad(pitch)));

        front = vmath::normalize(front_);

        right = vmath::normalize(vmath::cross(front, worldUp));
        up = vmath::normalize(vmath::cross(right, front));

        float distance = 2.0f * (position[1] - 0.0f);
        position[1] -= distance;
    }

    void updateInversCameraVectors_Mirror()
    {
        pitch = -pitch;
        vmath::vec3 front_ = vmath::vec3(
            cos(degToRad(yaw)) * cos(degToRad(pitch)),
            sin(degToRad(pitch)),
            sin(degToRad(yaw)) * cos(degToRad(pitch)));

        front = vmath::normalize(front_);

        right = vmath::normalize(vmath::cross(front, worldUp));
        up = vmath::normalize(vmath::cross(right, front));

        // float distance = 2.0f * (position[2] - 0.0f);
        // position[0] -= distance;
        position[0] = -position[0];
    }

    void updateResolution(float _width, float _height)
    {
        width = _width;
        height = _height;
    }

    void keyboardInputs(WPARAM keyPressed)
    {
        // in
        float velocity = movementSpeed * 0.1;
        if (keyPressed == 'w')
        {
            position = position + (front * velocity);
        }

        // left
        if (keyPressed == 'a')
        {
            position = position - (right * velocity);
        }

        // out
        if (keyPressed == 's')
        {
            position = position - (front * velocity);
        }

        // right
        if (keyPressed == 'd')
        {
            position = position + (right * velocity);
        }

        // up
        if (keyPressed == 'v')
        {
            position = position + (up * velocity);
        }

        // down
        if (keyPressed == ' ')
        {
            position = position - (up * velocity);
        }

        // if (keyPressed.shiftKey)
        // {
        //     movementSpeed = 6.5;
        // }
    }

    void invertPitch(void)
    {
        // pitch = -pitch;
        updateInversCameraVectors();
    }

    void invertPitch_Mirror(void)
    {
        // pitch = -pitch;
        updateInversCameraVectors_Mirror();
    }

    ///////////////// TO DO IN WINDOWS
    // Detects shift key up to decrease the movementSpeed
    // void inputOnKeyUp(event)
    // {
    //     if (event.code == 'ShiftLeft')
    //     {
    //         movementSpeed = 4.5;
    //     }
    // }

    void mouseInputs(float mouseX, float mouseY)
    {
        if (firstMouse)
        {
            lastX = mouseX;
            lastY = mouseY;
            firstMouse = FALSE;
        }
        float xoffset = mouseX - lastX;
        float yoffset = lastY - mouseY;
        lastX = mouseX;
        lastY = mouseY;

        BOOL constrainPitch = TRUE;
        xoffset *= mouseSensitivity;
        yoffset *= mouseSensitivity;

        yaw = fmod((yaw + xoffset), 360.0f);

        pitch += yoffset;

        if (constrainPitch)
        {
            if (pitch > 89.0)
                pitch = 89.0;
            if (pitch < -89.0)
                pitch = -89.0;
        }

        updateCameraVectors();
    }

    // Process mouse scroll
    void mouseScroll(float scrollDelta)
    {
        zoom -= (float)scrollDelta;

        PrintLog("Zoom = %f\n", zoom);

        if (zoom < -10000.0f)
            zoom = -10000.0f;

        if (zoom > 10000.0f)
            zoom = 10000.0f;

        perspectiveProjectionMatrix = vmath::perspective(45.0f + degToRad(zoom), (GLfloat)width / (GLfloat)height, 0.1f, 1000.0f);
    }

    float degToRad(float degrees)
    {
        return (degrees * M_PI / 180.0);
    }
};