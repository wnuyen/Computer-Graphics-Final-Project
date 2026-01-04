#version 330 core

in vec3 FragPos;
in vec4 FragPosLightSpace;

out vec4 color;

// Uniforms
uniform vec3 snowColor;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform sampler2D shadowMap;

// Shadow Calculation (Same as tree.frag)
float ShadowCalculation(vec4 fragPosLightSpace)
{
    // 1. Perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // 2. Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // 3. Keep shadow at 0.0 if outside the far plane
    if(projCoords.z > 1.0)
    return 0.0;
    // 4. Get closest depth value
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    // 5. Get current depth
    float currentDepth = projCoords.z;
    // 6. Bias to prevent moiré patterns
    float bias = 0.005;
    // 7. Check shadow
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

    return shadow;
}

void main(){
    // 1. Shape the Point (Circle)
    vec2 coord = gl_PointCoord - vec2(0.5);
    if(length(coord) > 0.5)
    discard;

    // 2. Define Normal
    // Snow falls flat or tumbles, but essentially reflects light from above.
    // We treat it as a sphere-like object or flat facing up.
    vec3 norm = vec3(0.0, 1.0, 0.0);

    // 3. Ambient Lighting
    float ambientStrength = 0.5; // Slightly higher for snow so it's not pitch black
    vec3 ambient = ambientStrength * lightColor;

    // 4. Diffuse Lighting
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // 5. Shadow Calculation
    float shadow = ShadowCalculation(FragPosLightSpace);

    // 6. Combine
    // Snow is white/bright, so we apply the color uniform
    vec3 result = (ambient + (1.0 - shadow) * diffuse) * snowColor;

    color = vec4(result, 1.0);
}