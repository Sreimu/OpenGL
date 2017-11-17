//
// Created by leisirui on 2017/11/2.
//

#ifndef OPENGL_MOUSE_H
#define OPENGL_MOUSE_H

// GLFW
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Mouse {

public :
    float xPos;
    float yPos;

    float xOffset;
    float yOffset;

    bool keys[10] = {0};

    float leftPressXPos;
    float leftPressYPos;

    float rightPressXPos;
    float rightPressYPos;

    Mouse(GLFWwindow *window);
    void ProcessMove(float xpos, float ypos);
    void ProcessClick(int button, int action);
    void GetCursorPos(float *xpos, float *ypos);
    void SetCursorPos(float xpos, float ypos);
    void Update();

private :
    float lastXPos;
    float lastYPos;

    bool firstMouse;
    GLFWwindow *window;

    bool leftPressed;
    bool rightPressed;
};


#endif //OPENGL_MOUSE_H
