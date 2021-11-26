#version 330 core
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
uniform float useAlbedoMap;
uniform sampler2D specularMap;
uniform float useSpecularMap;
uniform sampler2D normalMap;
uniform float useNormalMap;
uniform vec4 color;
uniform float shininess;

uniform vec4 cameraPos;
uniform float _Time;


float scrollSpeed = 2;
vec3 lightDir = vec3(-0.8, -0.7, -0.3);
vec3 lightColor = vec3(0.9, 0.9, 0.8);
float lightIntensity = 1;
float ambientStrength = 0.05f;

void main(){
	vec4 objectColor = color;
	vec3 normal = normalize(normalWorld);

	if(useNormalMap > 0){
		normal = texture(normalMap, texCoord).rgb;
		normal = normal * 2.0f - 1.0f;
		normal = normalize(tbn * normal);
	}
	
	vec3 cameraPos = (inverse(viewMatrix) * vec4(0,0,0,1)).xyz;
	vec3 viewDir = normalize(cameraPos.xyz - worldPos);
	vec3 halfwayDir = normalize(-lightDir + viewDir);
	
	float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
	float diff = max(0, dot(normal, -lightDir));
	
	vec3 ambientColor = ambientStrength * lightColor;
	vec3 diffuseColor = diff * lightColor * lightIntensity;
	vec3 specularColor = spec * lightColor * lightIntensity;
	if(useSpecularMap > 0){
		specularColor = specularColor * texture(specularMap, texCoord).rgb;
	}
	
	if(useAlbedoMap > 0){
		objectColor = objectColor * texture(albedoMap, texCoord);
	}
	
	FragColor = vec4((ambientColor + diffuseColor + specularColor) * objectColor.rgb, objectColor.a);
	//FragColor = vec4(normal, 1);
}