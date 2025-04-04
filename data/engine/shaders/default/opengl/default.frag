#version 450 core
#define APPLY_FOG(color) { \
	float distance = length(cameraPos - worldPos); \
	color = mix(color, fogColor, clamp(remap(distance, fogStart, fogEnd, 0, 1), 0, 1)); \
};

out vec4 FragColor;

in vec2 texCoord;
in vec3 normalLocal;
in vec3 normalWorld;
in vec3 worldPos;
in mat3 tbn;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform sampler2D albedoMap;
uniform bool useAlbedoMap;
uniform sampler2D specularMap;
uniform bool useSpecularMap;
uniform sampler2D normalMap;
uniform bool useNormalMap;
uniform vec4 color;
uniform float shininess;
uniform float fogStart;
uniform float fogEnd;
uniform vec4 fogColor;
uniform vec3 viewPosition;

//uniform vec4 cameraPos;
//uniform float _Time;


float scrollSpeed = 2;
vec3 lightDir = vec3(-0.8, -0.7, -0.3);
vec3 lightColor = vec3(0.9, 0.9, 0.8);
float lightIntensity = 2.28;
float ambientStrength = 0.2f;

float remap(float value, float min1, float max1, float min2, float max2) {
  return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}

void main(){
	vec4 objectColor = color;

	// Normalize inputs
	vec3 normal = normalize(normalWorld);
	vec3 lightDirection = normalize(-lightDir);
	vec3 viewDirection = normalize(viewPosition - worldPos);

	if(useNormalMap){
		normal = texture(normalMap, texCoord).rgb;
		normal = normal * 2.0f - 1.0f;
		normal = normalize(tbn * normal);
	}

	// Calculate diffuse shading
	float diff = max(0, dot(normal, -lightDir));
	vec3 diffuseColor = diff * lightColor * lightIntensity;

	// Calculate specular shading
	vec3 reflectDirection = reflect(-lightDirection, normal);
	float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), shininess);
	vec3 specularColor = spec * lightColor * lightIntensity;

	if(useSpecularMap){
		specularColor = specularColor * texture(specularMap, texCoord).rgb;
	}

	if(useAlbedoMap){
		objectColor = objectColor * texture(albedoMap, texCoord);
	}

	vec3 ambientColor = ambientStrength * lightColor;

	FragColor = vec4((diffuseColor + specularColor + ambientColor) * objectColor.rgb, objectColor.a);
	//APPLY_FOG(FragColor)
}