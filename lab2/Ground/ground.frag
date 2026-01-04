#version 330 core

in vec2 UV;
in vec3 fragmentColor;
in vec3 worldPosition;

out vec3 color;

uniform sampler2D textureSampler;
uniform vec3 cameraPosition;

// --- NEW UNIFORMS ---
uniform vec3 lightPos;
uniform vec3 lightColor;

void main(){
    // 1. Texture
    vec3 texColor = texture(textureSampler, UV * 10.0).rgb;

    // 2. Lighting Calculation
    // Ground is flat, so Normal is always UP (0,1,0)
    vec3 normal = vec3(0.0, 1.0, 0.0);

    // Ambient
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse
    vec3 lightDir = normalize(lightPos - worldPosition);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Combine lighting with texture
    vec3 finalColor = (ambient + diffuse) * texColor * fragmentColor;

    float dist = distance(worldPosition, cameraPosition);
    float fogStart = 500.0;
    float fogEnd = 950.0;
    float fogFactor = clamp((fogEnd - dist) / (fogEnd - fogStart), 0.0, 1.0);
    vec3 skyColor = vec3(0.2, 0.2, 0.25);

    color = mix(skyColor, finalColor, fogFactor);
}