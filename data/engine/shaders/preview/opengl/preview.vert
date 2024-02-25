#version 330 core
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;
out vec3 normalWorld;
out vec3 worldPos;

void main(){
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertex.x, vertex.y, vertex.z, 1.0);
	normalWorld = normalize((transpose(inverse(modelMatrix)) * vec4(normal, 0.0f)).xyz);
	worldPos = (modelMatrix * vec4(vertex.x, vertex.y, vertex.z, 1.0)).xyz;
}