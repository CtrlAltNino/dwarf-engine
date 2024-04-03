#version 450 core
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec3 biTangent;
layout (location = 4) in vec2 uvCoord;

void main(){
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertex.x, vertex.y, vertex.z, 1.0);
}