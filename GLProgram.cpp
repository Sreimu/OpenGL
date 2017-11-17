//
// Created by leisirui on 2017/11/3.
//

#include "GLProgram.h"

GLProgram::GLProgram(Shader *shader) {
    this->shader = shader;
}

void GLProgram::SetLight(Light l) {
    SetLightPosition(l.position);
    SetLightAmbient(l.ambient);
    SetLightDiffuse(l.diffuse);
    SetLightSpecular(l.specular);
}

void GLProgram::SetMaterial(Material m) {
    SetMaterialAmbient(m.ambient);
    SetMaterialDiffuse(m.diffuse);
    SetMaterialSpecular(m.specular);
    SetMaterialShininess(m.shininess);
}

void GLProgram::SetLightAmbient(glm::vec3 ambient) {
    glUniform3f(this->shader->GetUniformLocation("light.ambient"), ambient.x, ambient.y, ambient.z);
}

void GLProgram::SetLightDiffuse(glm::vec3 diffuse) {
    glUniform3f(this->shader->GetUniformLocation("light.diffuse"), diffuse.x, diffuse.y, diffuse.z);
}

void GLProgram::SetLightSpecular(glm::vec3 specular) {
    glUniform3f(this->shader->GetUniformLocation("light.specular"), specular.x, specular.y, specular.z);
}

void GLProgram::SetLightPosition(glm::vec3 position) {
    glUniform3f(this->shader->GetUniformLocation("light.position"), position.x, position.y, position.z);
}

void GLProgram::SetMaterialAmbient(glm::vec3 ambient) {
    glUniform3f(this->shader->GetUniformLocation("material.ambient"), ambient.x, ambient.y, ambient.z);
}

void GLProgram::SetMaterialDiffuse(glm::vec3 diffuse) {
    glUniform3f(this->shader->GetUniformLocation("material.diffuse"), diffuse.x, diffuse.y, diffuse.z);
}

void GLProgram::SetMaterialSpecular(glm::vec3 specular) {
    glUniform3f(this->shader->GetUniformLocation("material.specular"), specular.x, specular.y, specular.z);
}

void GLProgram::SetMaterialShininess(float s) {
    glUniform1f(this->shader->GetUniformLocation("material.shininess"), s);
}

void GLProgram::SetModelMatrix(glm::mat4 model) {
    glUniformMatrix4fv(this->shader->GetUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));
}

void GLProgram::SetViewMatrix(glm::mat4 view) {
    glUniformMatrix4fv(this->shader->GetUniformLocation("view"), 1, GL_FALSE, glm::value_ptr(view));
}

void GLProgram::SetProjection(glm::mat4 projection) {
    glUniformMatrix4fv(this->shader->GetUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection));
}

void GLProgram::SetViewPosition(glm::vec3 pos) {
    glUniform3f(this->shader->GetUniformLocation("viewPos"), pos.x, pos.y, pos.z);
}

void GLProgram::Use() {

    this->shader->Use();

}

GLint GLProgram::GetUniformLocation(const char* name) {
    return this->shader->GetUniformLocation(name);
}