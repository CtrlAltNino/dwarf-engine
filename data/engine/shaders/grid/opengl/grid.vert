#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
out vec3 worldPos;
out mat4 viewMatrixO;

void main(){
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	worldPos = (modelMatrix * vec4(aPos.xyz, 1.0)).xyz;
	viewMatrixO = viewMatrix;
}