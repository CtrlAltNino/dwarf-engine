#version 450 core

in vec3 FragNormal;
in vec3 FragPos;
out vec4 FragColor;

uniform vec3 viewPosition; // Camera position passed from CPU

// Fixed light direction (simulating sunlight)
const vec3 lightDir = normalize(vec3(0.5, 1.0, 0.3));
const vec3 lightColor = vec3(1.0, 1.0, 1.0);

void main() {
    vec3 N = normalize(FragNormal);

    // Diffuse lighting (Lambertian)
    float diff = max(dot(N, lightDir), 0.0);

    // Correct view vector calculation
    vec3 V = normalize(viewPosition - FragPos);

    // Blinn-Phong Specular (Halfway Vector)
    vec3 H = normalize(lightDir + V);
    float spec = pow(max(dot(N, H), 0.0), 32.0); // Shininess factor

    // Ambient + Diffuse + Specular
    vec3 ambient = vec3(0.2);
    vec3 diffuse = diff * lightColor;
    vec3 specular = spec * lightColor * 0.5;

    vec3 finalColor = ambient + diffuse + specular;
    FragColor = vec4(finalColor, 1.0);
}
