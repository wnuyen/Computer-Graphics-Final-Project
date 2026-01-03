#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColor;
layout(location = 2) in vec2 vertexUV;

out vec2 UV;
out vec3 fragmentColor;
out vec3 worldPosition;

uniform mat4 MVP;
uniform mat4 M; // Model Matrix

void main(){
    gl_Position = MVP * vec4(vertexPosition_modelspace, 1.0);

    // Pass UV to fragment shader
    UV = vertexUV;

    // Pass Color
    fragmentColor = vertexColor;

    // Calculate World Position for Fog logic
    worldPosition = (M * vec4(vertexPosition_modelspace, 1.0)).xyz;
}