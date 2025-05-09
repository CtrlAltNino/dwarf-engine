#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D hdrTexture;

vec3 agxTonemap(vec3 x) {
    const float A = 0.22;
    const float B = 0.30;
    const float C = 0.10;
    const float D = 0.20;
    const float E = 0.01;
    const float F = 0.30;

    return ((x * (A * x + C * B) + D * E) / (x * (A * x + B) + D * F)) - E / F;
}

// AGX tonemapping function
vec3 AGXTonemap2(vec3 color) {
    // Constants for AGX tonemapping
    const vec3 A = vec3(0.022, 0.015, 0.005);  // Some example values for A (should be small)
    const vec3 B = vec3(0.030, 0.030, 0.030);  // Example B (should be similar for all channels)
    const vec3 C = vec3(0.300, 0.300, 0.300);  // Example C (somewhat larger value for contrast)

    // Apply AGX tonemapping curve
    vec3 numerator = color * (A + color);
    vec3 denominator = color * (B + color) + C;
    color = numerator / denominator;

    return color;
}

void main() {
    // Fetch the HDR color from the texture
    vec3 hdrColor = texture(hdrTexture, TexCoords).rgb;

    // Apply AGX tonemapping
    vec3 mapped = AGXTonemap2(hdrColor);

    FragColor = vec4(mapped, 1.0);
}
