#version 330 core

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

in vec3 Normal;
in vec2 TexCoord;
in vec3 WorldPos;
in vec4 LightSpacePos;

out vec4 outColor;

uniform sampler2D texture1;
uniform sampler2D depthTexture;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

float calShadow(vec4 lightSpacePos);

void main()
{
    vec3 objectColor = texture(texture1, TexCoord).xyz;

    vec3 ambient = light.ambient * material.ambient;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - WorldPos);
    float diff = max(dot(norm, lightDir), 0);
    vec3 diffuse = light.diffuse * material.diffuse * diff;

    vec3 viewDir = normalize(viewPos - WorldPos);
    vec3 halfWayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfWayDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * material.specular;

//    vec4 lightSpacePos = LightSpacePos;
//    vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w;
//    projCoords = projCoords * 0.5 + 0.5;
//    float depth = texture(depthTexture, projCoords.xy).r;
//    outColor = vec4(vec3(projCoords.z), 1.0f);

    float shadow = calShadow(LightSpacePos);
    outColor = vec4((ambient + (diffuse + specular) * (1 - shadow)) * objectColor, 1.0f);
}

float calShadow(vec4 lightSpacePos) {
    vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w;
    projCoords = projCoords * 0.5 + 0.5;
    float depth = texture(depthTexture, projCoords.xy).r;
//    float z = depth * 2.0 - 1.0;
//    depth = (2.0 * nearPlane * farPlane) / (farPlane + nearPlane - z * (farPlane - nearPlane));
    return projCoords.z -0.005 > depth ? 1.0 : 0.0;
}