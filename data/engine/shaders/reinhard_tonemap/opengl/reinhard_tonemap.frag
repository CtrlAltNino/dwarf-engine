#version 450

layout(location = 0) in vec2 TexCoords;
layout(location = 0) out vec4 FragColor;

uniform sampler2D hdrTexture;
uniform float exposure; // Can be adjusted based on lighting conditions

void main() {
    // Fetch the HDR color from the texture
    vec3 hdrColor = texture(hdrTexture, TexCoords).rgb;

    // Apply exposure control
    vec3 mappedColor = hdrColor / (hdrColor + vec3(1.0));

    // Gamma correction (assuming output is sRGB)
    mappedColor = pow(mappedColor, vec3(1.0 / 2.2));

    FragColor = vec4(mappedColor, 1.0);
}
