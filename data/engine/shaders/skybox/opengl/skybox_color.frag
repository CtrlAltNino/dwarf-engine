#version 450 core

in vec3 vDirection;
out vec4 FragColor;

uniform vec3 u_SkyColor;

void main()
{
    FragColor = vec4(vDirection, 1.0);
}
