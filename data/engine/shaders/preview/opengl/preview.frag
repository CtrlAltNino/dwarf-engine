#version 330 core

out vec4 FragColor;
in vec3 normalWorld;
in vec3 worldPos;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main(){
	vec3 normal = normalize(normalWorld);
	vec3 cameraPos = (inverse(viewMatrix) * vec4(0,0,0,1)).xyz;
	vec3 viewDir = normalize(cameraPos.xyz - worldPos);
	float diff = max(0, dot(normal, viewDir));
	FragColor = vec4(diff, diff, diff, 1.0);
}