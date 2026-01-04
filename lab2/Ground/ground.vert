#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColor;
layout(location = 2) in vec2 vertexUV;

out vec2 UV;
out vec3 fragmentColor;
out vec3 worldPosition;

// --- FIX 1: Declare the output variable ---
out vec4 FragPosLightSpace;

uniform mat4 MVP;
uniform mat4 M;

// --- FIX 2: Declare the missing uniform ---
uniform mat4 lightSpaceMatrix;

void main(){
    // Calculate world position
    vec4 worldPos = M * vec4(vertexPosition_modelspace, 1.0);

    // Calculate position in Light Space (for shadow lookup)
    // This now works because both variables are declared above
    FragPosLightSpace = lightSpaceMatrix * worldPos;

    gl_Position = MVP * vec4(vertexPosition_modelspace, 1.0);

    // Pass UV to fragment shader
    UV = vertexUV;

    // Pass Color
    fragmentColor = vertexColor;

    // Calculate World Position for Fog logic
    worldPosition = worldPos.xyz;
}