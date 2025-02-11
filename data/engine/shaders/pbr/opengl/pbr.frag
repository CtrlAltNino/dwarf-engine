#version 450 core

// Input from vertex shader
in vec2 TexCoords;
in vec3 FragPos;
in mat3 TBN; // TBN matrix (tangent-space to world-space)

// Output to framebuffer
out vec4 FragColor;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

// Uniforms for textures
uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metalRoughnessMap; // Metalness (R) and Roughness (G)
uniform sampler2D emissiveMap;
uniform sampler2D aoMap; // Ambient Occlusion

// Uniforms for lighting
vec3 lightDir = vec3(-0.8, -0.7, -0.3);
vec3 lightColor = vec3(0.9, 0.9, 0.8);
uniform vec3 viewPosition;

// Constants
const float PI = 3.14159265359;

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

void main() {
    // Texture samples
    vec3 albedo = texture(albedoMap, TexCoords).rgb;
    vec3 emissive = texture(emissiveMap, TexCoords).rgb;
    float ao = texture(aoMap, TexCoords).r;

    vec3 tangentNormal = texture(normalMap, TexCoords).rgb * 2.0 - 1.0;
    vec3 N = normalize(TBN * tangentNormal); // Use the TBN matrix to transform tangent-space normal to world-space

    vec3 metalRoughness = texture(metalRoughnessMap, TexCoords).rgb;
    float metalness = metalRoughness.r;
    float roughness = clamp(metalRoughness.g, 0.05, 1.0); // Prevent 0 roughness for stability

    // Lighting calculations
    vec3 V = normalize(viewPosition - FragPos); // View direction
    vec3 L = normalize(-lightDir); // Light direction
    vec3 H = normalize(V + L); // Halfway vector

    float NdotL = max(dot(N, L), 0.0);
    float NdotV = max(dot(N, V), 0.0);

    // Fresnel equation (Schlick's approximation)
    vec3 F0 = mix(vec3(0.04), albedo, metalness); // Dielectrics have F0 ~ 0.04
    vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

    // Normal distribution function (GGX)
    float NDF = distributionGGX(N, H, roughness);

    // Geometry function (Smith's method)
    float G = geometrySmith(N, V, L, roughness);

    // Cook-Torrance BRDF
    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * NdotV * NdotL + 0.001; // Prevent division by zero
    vec3 specular = numerator / denominator;

    // Diffuse contribution (Lambertian)
    vec3 kD = vec3(1.0) - F;
    kD *= 1.0 - metalness; // Only non-metals have diffuse lighting
    vec3 diffuse = kD * albedo / PI;

    // Combine contributions
    vec3 radiance = lightColor * NdotL;
    vec3 lighting = (diffuse + specular) * radiance;

    // Apply AO
    lighting *= ao;

    // Add emissive map
    vec3 color = lighting + emissive;

    // Output final color
    FragColor = vec4(color, 1.0);
}
