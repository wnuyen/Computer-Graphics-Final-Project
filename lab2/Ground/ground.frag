#version 330 core

in vec2 UV;
in vec3 fragmentColor;
in vec3 worldPosition;

// --- FIX 1: Change output to vec4 (Standard for OpenGL) ---
out vec4 color;

uniform sampler2D textureSampler;
uniform vec3 cameraPosition;
uniform vec3 lightPos;
uniform vec3 lightColor;

in vec4 FragPosLightSpace;
uniform sampler2D shadowMap;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // 1. Perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // 2. Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // 3. Keep shadow at 0.0 if outside the far plane or too high
    if(projCoords.z > 1.0)
    return 0.0;
    // 4. Get closest depth value from light's perspective
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    // 5. Get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // 6. Bias to prevent shadow acne
    float bias = 0.005;
    // 7. Check whether current frag pos is in shadow
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

    return shadow;
}

void main(){
    // 1. Texture
    vec3 texColor = texture(textureSampler, UV * 10.0).rgb;

    // 2. Lighting Calculation
    vec3 normal = vec3(0.0, 1.0, 0.0);

    // Ambient
    float ambientStrength = 0.4;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse
    vec3 lightDir = normalize(lightPos - worldPosition);
    float diff = max(dot(normal, lightDir), 0.0);
    float diffuseStrength = 0.6;
    vec3 diffuse = diff * lightColor * diffuseStrength;

    // --- CALCULATE SHADOW ---
    float shadow = ShadowCalculation(FragPosLightSpace);

    // --- APPLY SHADOW ---
    // (Only diffuse is affected by shadow, ambient stays)
    vec3 lighting = (ambient + (1.0 - shadow) * diffuse) * texColor * fragmentColor;

    // --- FOG CALCULATION ---
    float dist = distance(worldPosition, cameraPosition);
    float fogStart = 1500.0;
    float fogEnd = 3000.0;
    float fogFactor = clamp((fogEnd - dist) / (fogEnd - fogStart), 0.0, 1.0);

    vec3 skyColor = vec3(0.2, 0.2, 0.25);

    // --- FIX 2 & 3: Mix the SHADOWED 'lighting' with fog, not 'finalColor' ---
    // We mix vec3s first, then convert to vec4 at the end
    vec3 result = mix(skyColor, lighting, fogFactor);

    color = vec4(result, 1.0);
}