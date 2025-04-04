#version 450

layout(location = 0) in vec2 TexCoords;
layout(location = 0) out vec4 FragColor;

uniform sampler2D hdrTexture;
uniform float exposure; // Can be adjusted based on lighting conditions

void main() {
    // Fetch the HDR color from the texture and apply exposure control
    vec3 hdrColor = exposure * texture(hdrTexture, TexCoords).rgb;

    // Apply Reinhard tonemapping
    hdrColor = hdrColor / (hdrColor + vec3(1.0));

    FragColor = vec4(hdrColor, 1.0);
}
