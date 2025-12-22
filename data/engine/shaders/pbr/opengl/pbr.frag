#version 450 core

// Input from vertex shader
in vec2 TexCoords;
in vec3 FragPos;
in mat3 TBN; // TBN matrix (tangent-space to world-space)

// Output to framebuffer
out vec4 FragColor;

struct DirectionalLight
{
    vec3 direction;
    float intensity;

    vec3 color;
    int castsShadows;

    int shadowIndex;
    vec3 padding;
};

struct PointLight
{
    vec3 position;
    float intensity;

    vec3 color;
    int castsShadows;

    float radius;
    int shadowIndex;
    vec2 padding;
};

layout(std430, binding = 3) buffer DirectionalLights
{
    DirectionalLight directionalLights[];
};

layout(std430, binding = 4) buffer PointLights
{
    PointLight pointLights[];
};

uniform int u_DirectionalLightCount;
uniform int u_PointLightCount;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform vec3 viewPosition;

// Uniforms for textures
uniform vec4 tint;

// Diffuse color
uniform bool hasAlbedoMap;
uniform sampler2D albedoMap;

// Normal map
uniform bool hasNormalMap;
uniform sampler2D normalMap;
uniform float normalStrength;

uniform bool hasMetalRoughnessMap;
uniform sampler2D metalRoughnessMap; // Metalness (R) and Roughness (G)
// Will be used when no metalness/roughness texture is provided
uniform float metalness;
uniform float roughness;

// Emission
uniform bool hasEmissiveMap;
uniform sampler2D emissiveMap;
uniform float emissionIntensity;

 // Ambient Occlusion
uniform bool hasAoMap;
uniform sampler2D aoMap;

// Uniforms for lighting
vec3 lightDir = vec3(-0.8, -0.7, -0.3);  // Normalized light direction
vec3 lightColor = vec3(1.0, 1.0, 1.0);   // White light
uniform float lightIntensity = 5;  // Intensity in LUX (lumens/m²)

// Constants
const float PI = 3.14159265359;
const float MIN_ROUGHNESS = 0.05;

// Functions
vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

float distributionGGX(vec3 N, vec3 H, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    return a2 / (PI * denom * denom);
}

float geometrySchlickGGX(float NdotV, float roughness) {
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float denom = NdotV * (1.0 - k) + k;
    return NdotV / denom;
}

float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = geometrySchlickGGX(NdotV, roughness);
    float ggx1 = geometrySchlickGGX(NdotL, roughness);
    return ggx1 * ggx2;
}

vec3 ApplyDirectionalLightPBR(
    DirectionalLight light,
    vec3 N,
    vec3 V,
    vec3 albedo,
    float metalness,
    float roughness
)
{
    vec3 L = normalize(-light.direction);
    vec3 H = normalize(V + L);

    float NdotL = max(dot(N, L), 0.0);
    float NdotV = max(dot(N, V), 0.0);

    if (NdotL <= 0.0)
        return vec3(0.0);

    // Fresnel
    vec3 F0 = mix(vec3(0.04), albedo, metalness);
    vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

    // BRDF terms
    float NDF = distributionGGX(N, H, roughness);
    float G   = geometrySmith(N, V, L, roughness);

    vec3 numerator = NDF * G * F;
    float denom = max(4.0 * NdotV * NdotL, 0.001);
    vec3 specular = numerator / denom;

    vec3 kD = (vec3(1.0) - F) * (1.0 - metalness);
    vec3 diffuse = kD * albedo / PI;

    float radianceFactor = light.intensity / PI;
    vec3 radiance = light.color * radianceFactor * NdotL;

    return (diffuse + specular) * radiance;
}

vec3 ApplyPointLightPBR(
    PointLight light,
    vec3 fragPos,
    vec3 N,
    vec3 V,
    vec3 albedo,
    float metalness,
    float roughness
)
{
    vec3 toLight = light.position - fragPos;
    float distance = length(toLight);

    if (distance > light.radius)
        return vec3(0.0);

    vec3 L = toLight / distance;
    vec3 H = normalize(V + L);

    float attenuation = 1.0 - (distance / light.radius);
    float NdotL = max(dot(N, L), 0.0);
    float NdotV = max(dot(N, V), 0.0);

    if (NdotL <= 0.0)
        return vec3(0.0);

    vec3 F0 = mix(vec3(0.04), albedo, metalness);
    vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

    float NDF = distributionGGX(N, H, roughness);
    float G   = geometrySmith(N, V, L, roughness);

    vec3 numerator = NDF * G * F;
    float denom = max(4.0 * NdotV * NdotL, 0.001);
    vec3 specular = numerator / denom;

    vec3 kD = (vec3(1.0) - F) * (1.0 - metalness);
    vec3 diffuse = kD * albedo / PI;

    float radianceFactor = light.intensity / PI;
    vec3 radiance = light.color * radianceFactor * NdotL * attenuation;

    return (diffuse + specular) * radiance;
}



void main()
{
    vec4 albedoSample = (hasAlbedoMap ? texture(albedoMap, TexCoords) : vec4(1.0)) * tint;
    vec3 albedo = albedoSample.rgb;

    vec3 emissive = hasEmissiveMap ? texture(emissiveMap, TexCoords).rgb : vec3(0.0);
    float ao = hasAoMap ? texture(aoMap, TexCoords).r : 1.0;

    vec3 tangentNormal = texture(normalMap, TexCoords).rgb * 2.0 - 1.0;
    tangentNormal.xy *= clamp(normalStrength, 0.0, 1.0);
    vec3 N = hasNormalMap ? normalize(TBN * tangentNormal) : normalize(TBN[2]);

    vec3 metalRoughness = hasMetalRoughnessMap
        ? texture(metalRoughnessMap, TexCoords).rgb
        : vec3(metalness, roughness, 0.0);

    float metal = metalRoughness.r;
    float rough = clamp(metalRoughness.g, MIN_ROUGHNESS, 1.0);

    vec3 V = normalize(viewPosition - FragPos);

    vec3 lighting = vec3(0.0);

    // Directional lights
    for (int i = 0; i < u_DirectionalLightCount; ++i)
    {
        lighting += ApplyDirectionalLightPBR(
            directionalLights[i],
            N,
            V,
            albedo,
            metal,
            rough
        );
    }

    // Point lights
    for (int i = 0; i < u_PointLightCount; ++i)
    {
        lighting += ApplyPointLightPBR(
            pointLights[i],
            FragPos,
            N,
            V,
            albedo,
            metal,
            rough
        );
    }

    lighting *= ao;

    vec3 color = lighting + emissive * emissionIntensity;

    FragColor = vec4(color, albedoSample.a);
}