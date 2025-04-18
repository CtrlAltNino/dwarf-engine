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

out vec2 texCoord;
out vec3 normalLocal;
out vec3 normalWorld;
out vec3 worldPos;
out mat3 tbn;
float amplitude = 0.2;
float speed = 2;

void main(){
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertex.x, vertex.y, vertex.z, 1.0);
	texCoord = uvCoord;
	normalLocal = normal;
	worldPos = vec3(modelMatrix * vec4(vertex, 1.0));

	normalWorld = normalize(mat3(transpose(inverse(modelMatrix))) * normal);
	vec3 T = normalize(mat3(modelMatrix) * tangent);
	vec3 B = normalize(mat3(modelMatrix) * biTangent);

	tbn = mat3(T, B, normalWorld);
}