//
// Created by leisirui on 2017/11/2.
//

#ifndef OPENGL_LIGHT_H
#define OPENGL_LIGHT_H

// GL Includes
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Light {
public :
    glm::vec3 position;
    glm::vec3 color;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};


#endif //OPENGL_LIGHT_H
