#version 330 core
out vec4 FragColor;
in vec3 localPos;

uniform sampler2D equirectangularMap;

const float PI = 3.14159265359;

void main()
{
    vec3 dir = normalize(localPos);

    // Convert 3D direction to 2D UV (longitude/latitude)
    float phi = atan(dir.z, dir.x);
    float theta = asin(-dir.y);

    vec2 uv = vec2(phi / (2.0 * PI) + 0.5, theta / PI + 0.5);
    vec3 color = texture(equirectangularMap, uv).rgb;

    FragColor = vec4(color, 1.0);
}
