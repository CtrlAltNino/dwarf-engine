// grid_postprocess.frag
#version 330 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D uSceneColor;
uniform sampler2D uSceneDepth;

uniform mat4 uInverseViewProjection;
uniform mat4 uInverseView;
uniform vec3 uCameraPosition;
uniform float uGridHeight;
uniform float uOpacity;

vec3 WorldPosFromDepth(float depth, vec2 uv) {
    float z = depth * 2.0 - 1.0; // Convert [0,1] to [-1,1]
    vec4 clip = vec4(uv * 2.0 - 1.0, z, 1.0);
    vec4 world = uInverseViewProjection * clip;
    return world.xyz / world.w;
}

void main() {
    vec4 sceneColor = texture(uSceneColor, TexCoords);
    float depth = texture(uSceneDepth, TexCoords).r;

    vec3 rayTarget;
    bool background = false;

    if (depth >= 1.0) {
        // Background pixel: just pretend it's on the far plane
        rayTarget = WorldPosFromDepth(1.0, TexCoords);
        background = true;
    } else {
        rayTarget = WorldPosFromDepth(depth, TexCoords);
    }

    vec3 rayDir = normalize(rayTarget - uCameraPosition);

    // Intersect ray with Y = uGridHeight
    float t = (uGridHeight - uCameraPosition.y) / rayDir.y;
    if (t <= 0.0) {
        FragColor = sceneColor;
        return;
    }

    vec3 gridIntersect = uCameraPosition + rayDir * t;

    // Prevent drawing grid through geometry
    if (!background && t > length(rayTarget - uCameraPosition)) {
        FragColor = sceneColor;
        return;
    }

    // Grid pattern
    float gridScale = 1.0;
    vec2 coord = gridIntersect.xz / gridScale;
    vec2 grid = abs(fract(coord - 0.5) - 0.5) / fwidth(coord);
    float line = min(grid.x, grid.y);
    float gridAlpha = 1.0 - clamp(line, 0.0, 1.0);

    float distanceXZ = length(gridIntersect.xz - uCameraPosition.xz); // Ignore Y axis
    float fade = exp(-0.06 * distanceXZ); // Adjust constant for fade rate

    gridAlpha *= fade * uOpacity;

    vec3 gridColor = mix(sceneColor.rgb, vec3(0.6, 0.6, 0.6), gridAlpha);
    FragColor = vec4(gridColor, 1.0);
}