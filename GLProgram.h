//
// Created by leisirui on 2017/11/3.
//

#ifndef OPENGL_GLPROGRAM_H
#define OPENGL_GLPROGRAM_H

// GL Includes
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Light.h"
#include "Material.h"

class GLProgram {
public:
    GLProgram(Shader *shader);

    Shader* shader;
    GLuint VAO;

    void Use();
    void SetVAO();

    void SetMaterialAmbient(glm::vec3);
    void SetMaterialDiffuse(glm::vec3);
    void SetMaterialSpecular(glm::vec3);

    void SetLightAmbient(glm::vec3);
    void SetLightDiffuse(glm::vec3);
    void SetLightSpecular(glm::vec3);
    void SetLightPosition(glm::vec3);

    void SetMaterialShininess(float);
    void SetMaterial(Material m);
    void SetLight(Light l);

    void SetViewMatrix(glm::mat4);
    void SetModelMatrix(glm::mat4);
    void SetProjection(glm::mat4);
    void SetViewPosition(glm::vec3);

    GLint GetUniformLocation(const char* name);
};


#endif //OPENGL_GLPROGRAM_H
