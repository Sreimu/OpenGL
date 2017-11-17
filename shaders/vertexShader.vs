#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

out vec3 Normal;
out vec2 TexCoord;
out vec3 WorldPos;
out vec4 LightSpacePos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightMatrix;

void main()
{
    Normal = normal;
    TexCoord = texCoord;
    gl_Position = projection * view * model * vec4(position, 1.0);
    WorldPos = vec3(model * vec4(position, 1.0));
    LightSpacePos = lightMatrix * model * vec4(position, 1.0f);
}