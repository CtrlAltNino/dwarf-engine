#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D hdrTexture;
uniform float exposure;

vec3 agxTonemap(vec3 x) {
    const float A = 0.22;
    const float B = 0.30;
    const float C = 0.10;
    const float D = 0.20;
    const float E = 0.01;
    const float F = 0.30;

    return ((x * (A * x + C * B) + D * E) / (x * (A * x + B) + D * F)) - E / F;
}

void main() {
    vec3 hdrColor = texture(hdrTexture, TexCoords).rgb;

    hdrColor *= exposure;

    // Apply AGX tonemapping
    vec3 mapped = agxTonemap(hdrColor);

    // Gamma correction
    mapped = pow(mapped, vec3(1.0 / 2.2));

    FragColor = vec4(mapped, 1.0);
}
