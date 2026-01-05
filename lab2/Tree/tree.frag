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
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    if(projCoords.z > 1.0)
    return 0.0;
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    float bias = 0.005;
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

    // 3. Calculate Shadow
    float shadow = ShadowCalculation(FragPosLightSpace);

    // 4. Apply Shadow (Shadow affects Diffuse, not Ambient)
    vec3 lighting = (ambient + (1.0 - shadow) * diffuse) * fragmentColor;

    color = vec4(lighting, 1.0);
}