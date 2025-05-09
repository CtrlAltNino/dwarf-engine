#version 450 core

in vec3 vDirection;
out vec4 FragColor;

uniform vec3 u_SkyColor;
uniform float u_Exposure;

void main()
{
    FragColor = vec4(u_SkyColor * u_Exposure, 1.0);
}
