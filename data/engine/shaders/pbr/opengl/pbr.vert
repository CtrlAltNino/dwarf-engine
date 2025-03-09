#version 450 core
uniform float _Time;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec3 biTangent;
layout (location = 4) in vec2 uvCoord;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;
out mat3 TBN;

void main(){
	vec4 worldPos = modelMatrix * vec4(vertex, 1.0);
    FragPos = worldPos.xyz;  // Store world space position
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertex.x, vertex.y, vertex.z, 1.0);

	TexCoords = uvCoord;

	vec3 N = normalize(mat3(transpose(inverse(modelMatrix))) * normal);
	vec3 T = normalize(mat3(modelMatrix) * tangent);
	vec3 B = normalize(mat3(modelMatrix) * biTangent);

	TBN = mat3(T, B, N);
}