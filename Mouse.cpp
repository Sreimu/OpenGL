//
// Created by leisirui on 2017/11/2.
//

#include "Mouse.h"

Mouse::Mouse(GLFWwindow *window) {
    this->window = window;
}

void Mouse::ProcessMove(float xpos, float ypos) {
    this->xPos = xpos;
    this->yPos = ypos;

    if(firstMouse)
    {
        firstMouse = false;
        lastXPos = (GLfloat)xpos;
        lastYPos = (GLfloat)ypos;
        return;
    }

    xOffset = (GLfloat)xpos - lastXPos;
    yOffset = lastYPos - (GLfloat)ypos;  // Reversed since y-coordinates go from bottom to left

    lastXPos = (GLfloat)xpos;
    lastYPos = (GLfloat)ypos;
}

void Mouse::ProcessClick(int button, int action) {
    if (action == GLFW_PRESS)
        keys[button] = true;
    else if (action == GLFW_RELEASE)
        keys[button] = false;
}

void Mouse::GetCursorPos(float *xpos, float *ypos) {
    *xpos = xPos;
    *ypos = yPos;
}

void Mouse::SetCursorPos(float xpos, float ypos) {
    glfwSetCursorPos(window, xpos, ypos);
}

void Mouse::Update()
{
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    this->ProcessMove((float)xpos, (float)ypos);

    if (keys[GLFW_MOUSE_BUTTON_LEFT] && !leftPressed) {
        leftPressXPos = (float)xpos;
        leftPressYPos = (float)ypos;
        leftPressed = true;
    }

    if (!keys[GLFW_MOUSE_BUTTON_LEFT] && leftPressed)
        leftPressed = false;

    if (keys[GLFW_MOUSE_BUTTON_RIGHT] && !rightPressed) {
        rightPressXPos = (float)xpos;
        rightPressYPos = (float)ypos;
        rightPressed = true;
    }

    if (!keys[GLFW_MOUSE_BUTTON_RIGHT] && rightPressed)
        rightPressed = false;
}