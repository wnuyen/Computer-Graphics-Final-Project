#version 330 core

// Inputs from vertex shader
in vec3 fragmentColor;
in vec3 Normal;
in vec3 FragPos;

out vec4 color;

// Uniforms passed from tree.h
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
    // 4. Get closest depth value
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    // 5. Get current depth
    float currentDepth = projCoords.z;
    // 6. Bias
    float bias = 0.005;
    // 7. Check shadow
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

    return shadow;
}

void main(){
    // 1. Ambient Lighting
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * lightColor;

    // 2. Diffuse Lighting
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // --- REMOVED REDUNDANT 'result' CALCULATION ---

    // 3. Calculate Shadow
    float shadow = ShadowCalculation(FragPosLightSpace);

    // 4. Apply Shadow (Shadow affects Diffuse, not Ambient)
    vec3 lighting = (ambient + (1.0 - shadow) * diffuse) * fragmentColor;

    // --- FIX: Output the 'lighting' variable, NOT the unshadowed result ---
    color = vec4(lighting, 1.0);
}