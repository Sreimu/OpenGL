#version 330 core

out vec4 outColor;

in vec2 TexCoord;

uniform sampler2D depthTexture;
uniform float nearPlane;
uniform float farPlane;

void main()
{
    float depth = texture(depthTexture, TexCoord).r;
    float z = depth * 2.0 - 1.0;
    depth = (2.0 * nearPlane * farPlane) / (farPlane + nearPlane - z * (farPlane - nearPlane));
    outColor = vec4(vec3(depth), 1.0f);
}