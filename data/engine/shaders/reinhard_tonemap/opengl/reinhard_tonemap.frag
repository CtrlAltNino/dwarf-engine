#version 450

layout(location = 0) in vec2 TexCoords;
layout(location = 0) out vec4 FragColor;

uniform sampler2D hdrTexture;
uniform float exposure; // Can be adjusted based on lighting conditions

void main() {
    // Fetch the HDR color from the texture
    vec3 hdrColor = texture(hdrTexture, TexCoords).rgb;

    // Apply exposure control
    hdrColor *= exposure;

    // Gamma correction (assuming output is sRGB)
    hdrColor = pow(hdrColor, vec3(1.0 / 2.2));

    FragColor = vec4(hdrColor, 1.0);
}
