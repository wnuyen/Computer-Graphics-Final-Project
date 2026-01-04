#version 330 core

// Inputs from vertex shader
in vec3 fragmentColor;
in vec3 Normal;
in vec3 FragPos;

out vec4 color;

// Uniforms passed from tree.h
uniform vec3 lightPos;
uniform vec3 lightColor;

void main(){
    // 1. Ambient Lighting (Base light so objects aren't pitch black in shadow)
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * lightColor;

    // 2. Diffuse Lighting (Directional light from the sun)
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    // Calculate impact of light based on angle (dot product)
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // 3. Combine results
    // Multiply by the object's color (fragmentColor)
    vec3 result = (ambient + diffuse) * fragmentColor;

    color = vec4(result, 1.0);
}