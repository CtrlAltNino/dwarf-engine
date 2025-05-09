#version 450

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D hdrTexture;
uniform float exposure; // Can be adjusted based on lighting conditions

void main() {
    FragColor = vec4(texture(hdrTexture, TexCoords).rgb * exposure, 1.0);
}
