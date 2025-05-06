#version 450 core

layout (location = 0) in vec3 aPosition;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 vDirection;

void main()
{
    vDirection = aPosition;
    vec4 pos =  projectionMatrix * viewMatrix * vec4(aPosition, 1.0);
    gl_Position = pos.xyww; // Trick to ensure depth = 1.0 (far plane)
}
