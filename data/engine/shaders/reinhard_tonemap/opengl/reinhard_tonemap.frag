#version 450

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D hdrTexture;

void main() {
    // Fetch the HDR color from the texture and apply exposure control
    vec3 hdrColor = texture(hdrTexture, TexCoords).rgb;

    // Apply Reinhard tonemapping
    hdrColor = hdrColor / (hdrColor + vec3(1.0));

    FragColor = vec4(hdrColor, 1.0);
}
