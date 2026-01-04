#version 330 core

// Input vertex data
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColor;
layout(location = 2) in vec3 vertexNormal; // This matches the new buffer in tree.h

// Output data
out vec3 fragmentColor;
out vec3 Normal;   // Required for lighting
out vec3 FragPos;  // Required for lighting

uniform mat4 MVP;
uniform mat4 M; // Model Matrix

uniform mat4 lightSpaceMatrix;
out vec4 FragPosLightSpace;

void main(){

    // Calculate world position
    vec4 worldPos = M * vec4(vertexPosition_modelspace, 1.0);

    // Calculate position in Light Space (for shadow lookup)
    FragPosLightSpace = lightSpaceMatrix * worldPos;

    gl_Position = MVP * vec4(vertexPosition_modelspace, 1);

    fragmentColor = vertexColor;

    // Calculate Normal in World Space
    // We cast to mat3 to remove translation from the Model Matrix
    Normal = mat3(transpose(inverse(M))) * vertexNormal;

    // Calculate Fragment Position in World Space
    FragPos = vec3(M * vec4(vertexPosition_modelspace, 1.0));
}