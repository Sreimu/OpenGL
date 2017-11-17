//
// Created by leisirui on 2017/11/2.
//

#ifndef OPENGL_MATERIAL_H
#define OPENGL_MATERIAL_H

// GL Includes
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Material {
public:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};


#endif //OPENGL_MATERIAL_H
