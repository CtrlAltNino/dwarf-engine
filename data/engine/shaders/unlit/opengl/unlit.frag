#version 450 core

in vec2 FragUV;
out vec4 FragColor;

uniform sampler2D albedoMap;
uniform vec4 color;

void main() {
    FragColor = color * texture(albedoMap, FragUV);
}
