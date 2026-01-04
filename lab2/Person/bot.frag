#version 330 core

in vec3 fragmentNormal;
out vec4 color;

uniform vec3 lightPosition;
uniform vec3 lightIntensity; // This receives 'sunColor' from main.cpp

void main() {
    // Basic diffuse lighting setup
    vec3 n = normalize(fragmentNormal);
    vec3 l = normalize(lightPosition);

    // Calculate brightness based on angle between normal and light
    // clamp(..., 0.2, 1.0) ensures the back of the model isn't pitch black (ambient light)
    float brightness = clamp(dot(n, l), 0.2, 1.0);

    // Apply the color passed from C++ (Orange/Yellow/Blue based on sun time)
    color = vec4(lightIntensity * brightness, 1.0);
}