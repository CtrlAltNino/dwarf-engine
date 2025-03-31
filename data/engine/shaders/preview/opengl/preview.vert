#version 450 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 FragPos;
out vec3 FragNormal;

void main(){
    // Transform vertex position into world space
    FragPos = vec3(modelMatrix * vec4(vertex, 1.0));

    // Transform normal correctly (normal matrix is the inverse transpose of model)
    mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
    FragNormal = normalize(normalMatrix * normal);

    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertex, 1.0);
}