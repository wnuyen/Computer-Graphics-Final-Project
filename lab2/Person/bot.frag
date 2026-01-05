#version 330 core

// Inputs from the vertex shader
in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;

// Output color
out vec4 color;

// Uniforms
uniform vec3 lightPosition;
uniform vec3 lightIntensity;
uniform sampler2D diffuseTexture;

void main() {
    // Red = (1.0, 0.0, 0.0)
    vec3 objectColor = vec3(1.0, 0.0, 0.0);

    vec3 n = normalize(vNormal);
    vec3 l = normalize(lightPosition - vPosition);

    // Calculate diffuse brightness
    float diffuseStrength = max(dot(n, l), 0.0);

    vec3 finalColor = (lightIntensity * diffuseStrength + vec3(0.1)) * objectColor;

    color = vec4(finalColor, 1.0);
}