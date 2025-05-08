#version 450 core

in vec3 vDirection;
out vec4 FragColor;

uniform vec3 u_SunDirection;
uniform float u_SkyExposure;

const vec3 horizonColor = vec3(0.8, 0.9, 1.0);
const vec3 zenithColor  = vec3(0.05, 0.2, 0.5);
const vec3 sunColor     = vec3(10.0, 9.0, 5.0) * 1; // Increased intensity for HDR

const float sunAngularRadius = radians(0.53);
const float sunDiskCos = cos(sunAngularRadius);
const float sunGlowFalloff = 400.0;

void main()
{
    vec3 dir = normalize(vDirection);

    // Sky gradient based on view direction
    float t = clamp(-dir.y * 0.5 + 0.5, 0.0, 1.0);
    vec3 skyGradient = mix(horizonColor, zenithColor, t);

    // Sun disk and glow
    float cosTheta = dot(dir, normalize(u_SunDirection));
    float sunDisk = smoothstep(sunDiskCos - 0.0005, sunDiskCos + 0.0005, cosTheta);
    float sunGlow = exp((cosTheta - 1.0) * sunGlowFalloff);

    vec3 color = skyGradient;
    color += sunColor * sunGlow;
    color = mix(color, sunColor, sunDisk);

    // Apply exposure
    color *= u_SkyExposure;

    FragColor = vec4(color, 1.0);
}
