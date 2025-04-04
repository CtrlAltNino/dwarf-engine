#version 450

layout(location = 0) out vec4 FragColor;
layout(location = 0) in vec2 TexCoords;

uniform sampler2D hdrTexture;
uniform float exposure;

vec3 RRTAndODTFit(vec3 v) {
    vec3 a = v * (v + 0.0245786) - 0.000090537;
    vec3 b = v * (v * 0.983729 + 0.4329510) + 0.238081;
    return a / b;
}

vec3 ACESFilm(vec3 color) {
    return clamp(RRTAndODTFit(color), 0.0, 1.0);
}

void main() {
    // Fetch the HDR color from the texture and apply exposure control
    vec3 hdrColor = exposure * texture(hdrTexture, TexCoords).rgb;

    // Apply ACES tonemapping
    vec3 ldrColor = ACESFilm(hdrColor);

    FragColor = vec4(ldrColor, 1.0);
}
