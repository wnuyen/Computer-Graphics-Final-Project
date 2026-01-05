#version 330 core

// Input vertex data (updated by CPU every frame)
layout(location = 0) in vec3 vertexPosition_worldspace;

// Uniforms
uniform mat4 MVP;
uniform mat4 lightSpaceMatrix;

// Outputs to Fragment Shader
out vec3 FragPos;
out vec4 FragPosLightSpace;

void main(){
    // 1. Pass World Position to Fragment Shader
    FragPos = vertexPosition_worldspace;

    // 2. Calculate Light Space Position (for shadow map lookup)
    FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);

    // 3. Calculate Clip Space Position
    gl_Position = MVP * vec4(vertexPosition_worldspace, 1.0);

    // 4. Point Size Calculation
    // Only calculate size for points in front of the camera
    if (gl_Position.w > 0.0) {
        gl_PointSize = 400.0 / gl_Position.w;
    } else {
        gl_PointSize = 0.0;
    }
}