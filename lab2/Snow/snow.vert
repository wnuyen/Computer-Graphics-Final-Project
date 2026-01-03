#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_worldspace;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;

void main(){
    // Inside snow.vert main()

    gl_Position = MVP * vec4(vertexPosition_worldspace, 1.0);

    // Fix: Only calculate size for points in front of the camera (w > 0)
    if (gl_Position.w > 0.0) {
        gl_PointSize = 400.0 / gl_Position.w;
    } else {
        gl_PointSize = 0.0; // Hide points behind the camera
    }
}