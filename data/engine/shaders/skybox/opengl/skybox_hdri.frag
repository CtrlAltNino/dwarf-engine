#version 450 core

in vec3 vDirection;
out vec4 FragColor;

uniform samplerCube u_Skybox;
uniform float u_Exposure;
uniform float u_Rotation;

void main()
{
    // Convert degrees to radians
    float radians = radians(u_Rotation);

    // Create a 3x3 rotation matrix around the Y axis
    mat3 rotationMatrix = mat3(
        cos(radians), 0.0, -sin(radians),
        0.0,         1.0,  0.0,
        sin(radians), 0.0, cos(radians)
    );

    // Rotate the direction vector
    vec3 rotatedDirection = rotationMatrix * normalize(vDirection);

    // Sample from the cubemap using the rotated direction
    vec3 color = texture(u_Skybox, rotatedDirection).rgb;
    FragColor = vec4(color * u_Exposure, 1.0);
}