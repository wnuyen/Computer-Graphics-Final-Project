#version 330 core

// Input attributes matches person.h bindMesh order
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexUV;
layout(location = 3) in vec4 vertexJoints;  // Bone indices
layout(location = 4) in vec4 vertexWeights; // Bone weights

// Uniforms passed from person.h -> render()
uniform mat4 MVP;
uniform mat4 jointMatrices[25]; // Array size matches joints in bot.gltf (25 joints)

// Output to fragment shader
out vec3 fragmentNormal;

void main() {
    // 1. Calculate Skinning Matrix
    // The model is animated, so we combine the transforms of the 4 influencing bones
    // based on their weights.
    mat4 skinMatrix =
    vertexWeights.x * jointMatrices[int(vertexJoints.x)] +
    vertexWeights.y * jointMatrices[int(vertexJoints.y)] +
    vertexWeights.z * jointMatrices[int(vertexJoints.z)] +
    vertexWeights.w * jointMatrices[int(vertexJoints.w)];

    // 2. Calculate final vertex position
    // MVP * SkinMatrix * LocalPosition
    gl_Position = MVP * skinMatrix * vec4(vertexPosition, 1.0);

    // 3. Transform the normal using the skin matrix (for lighting)
    // We only take the rotation part of the matrix (mat3)
    fragmentNormal = mat3(skinMatrix) * vertexNormal;
}