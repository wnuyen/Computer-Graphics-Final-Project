#version 330 core

in vec2 UV;
in vec3 fragmentColor;
in vec3 worldPosition;

out vec3 color;

uniform sampler2D textureSampler;
uniform vec3 cameraPosition;

void main(){
    // 1. Sample texture
    vec3 textureColor = texture(textureSampler, UV * 10.0).rgb;

    // 2. Fog Logic
    float dist = distance(worldPosition, cameraPosition);

    // --- FIX: Adjusted Fog Distances ---
    // Start fogging at 500 units away (so the ground below you at 450 is clear)
    float fogStart = 500.0;
    // End fog at 950 units (just before the camera's zFar clip plane of 1000)
    float fogEnd = 950.0;
    // -----------------------------------

    float fogFactor = clamp((fogEnd - dist) / (fogEnd - fogStart), 0.0, 1.0);

    // Sky color (Matches your main.cpp glClearColor)
    vec3 skyColor = vec3(0.2, 0.2, 0.25);

    color = mix(skyColor, textureColor * fragmentColor, fogFactor);
}