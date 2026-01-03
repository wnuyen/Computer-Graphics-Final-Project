#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexColor;
layout(location = 2) in vec2 vertexUV;

out vec3 color;
out vec2 UV;  // Must match fragment shader input

uniform mat4 MVP;

void main() {
    gl_Position = MVP * vec4(vertexPosition, 1.0);
    color = vertexColor;
    UV = vertexUV; // pass UV to fragment shader
}
