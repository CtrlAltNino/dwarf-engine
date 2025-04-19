#version 450

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D hdrTexture;
uniform float exposure; // Can be adjusted based on lighting conditions

float A = 0.15;
float B = 0.50;
float C = 0.10;
float D = 0.20;
float E = 0.02;
float F = 0.30;

vec3 uncharted2ToneMapping(vec3 hdrColor){
    return ((hdrColor * (A * hdrColor + C * B) + D * E) / (hdrColor * (A * hdrColor + B) + D * F)) - E / F;
}

void main() {
    // Fetch the HDR color from the texture and apply exposure control
    vec3 hdrColor = exposure * texture(hdrTexture, TexCoords).rgb;

    // Apply Uncharted2 tonemapping
    hdrColor = uncharted2ToneMapping(hdrColor);

    FragColor = vec4(hdrColor, 1.0);
}
