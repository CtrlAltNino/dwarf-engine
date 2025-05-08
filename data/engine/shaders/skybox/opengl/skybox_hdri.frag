#version 450 core

in vec3 vDirection;
out vec4 FragColor;

uniform samplerCube u_Skybox;

void main()
{
    vec3 color = texture(u_Skybox, normalize(vDirection)).rgb;
    FragColor = vec4(color * 50, 1.0);
}
