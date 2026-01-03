#version 330 core

out vec3 color;
uniform vec3 snowColor;

void main(){
    // Make the point round
    // gl_PointCoord goes from (0,0) to (1,1) within the point
    vec2 coord = gl_PointCoord - vec2(0.5);

    // If distance from center > 0.5, discard (makes a circle)
    if(length(coord) > 0.5)
    discard;

    color = snowColor;
}