#version 330 core
out vec4 FragColor;
in vec3 worldPos;
in mat4 viewMatrixO;

// Edge length of the squares
float gridSize = 1.0f;
// Offset for each axis (Normalized between 0 and 1)
vec2 offset = vec2(0);
// Thickness of the lines (Between 0 and 1)
float thickness = 0.05f;
// Color of the grid
vec3 color = vec3(1.0f, 1.0f, 1.0f);
// Base transparency of the grid
float transparency = 0.2f;
// Where inside of the line the fading should begin (Between 0 and 1)
float fade = 0.5f;
// Power to what the fading is risen to
float fadePower = 2.0f;

float remap(float x, float a, float b, float c, float d) {
  return c + (x - a) * (d - c) / (b - a);
}

void main(){
	vec2 manipulatedCoords = (worldPos.xz + offset) / gridSize;
	float factorX =  pow(1.0f - smoothstep(fade * (thickness / 2.0f), (thickness / 2.0f), min(fract(manipulatedCoords.x), ceil(manipulatedCoords.x) - manipulatedCoords.x)), fadePower);
	float factorY =  pow(1.0f - smoothstep(fade * (thickness / 2.0f), (thickness / 2.0f), min(fract(manipulatedCoords.y), ceil(manipulatedCoords.y) - manipulatedCoords.y)), fadePower);
	float distance_frag_camera = length((inverse(viewMatrixO) * vec4(0, 0, 0, 1)).xz - worldPos.xz);
	float distanceFade = clamp(remap(distance_frag_camera, 30, 50, 1, 0),0,1);
	FragColor = vec4(color, clamp(max(factorX, factorY), 0, 1) * transparency * distanceFade);
}