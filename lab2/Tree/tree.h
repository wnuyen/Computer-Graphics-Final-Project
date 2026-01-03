#pragma once
#include <vector>
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <render/shader.h>

struct Tree {
    std::vector<glm::vec3> treePositions;

    // 1. Geometry
    // CHANGED: Size increased from 108 to 126 to include the bottom face
    GLfloat vertex_buffer_data[126] = {
        // --- TRUNK WALLS (Brown) ---

        // Front face
        -0.5f, 0.0f,  0.5f,   0.5f, 0.0f,  0.5f,   0.5f, 5.0f,  0.5f,
        -0.5f, 0.0f,  0.5f,   0.5f, 5.0f,  0.5f,  -0.5f, 5.0f,  0.5f,

        // Back face
         0.5f, 0.0f, -0.5f,  -0.5f, 0.0f, -0.5f,  -0.5f, 5.0f, -0.5f,
         0.5f, 0.0f, -0.5f,  -0.5f, 5.0f, -0.5f,   0.5f, 5.0f, -0.5f,

        // Left face
        -0.5f, 0.0f, -0.5f,  -0.5f, 0.0f,  0.5f,  -0.5f, 5.0f,  0.5f,
        -0.5f, 0.0f, -0.5f,  -0.5f, 5.0f,  0.5f,  -0.5f, 5.0f, -0.5f,

        // Right face
         0.5f, 0.0f,  0.5f,   0.5f, 0.0f, -0.5f,   0.5f, 5.0f, -0.5f,
         0.5f, 0.0f,  0.5f,   0.5f, 5.0f, -0.5f,   0.5f, 5.0f,  0.5f,

        // --- NEW: TRUNK BOTTOM (Floor) ---
        // This closes the hole so you can't see inside when looking up
        -0.5f, 0.0f, -0.5f,   0.5f, 0.0f, -0.5f,   0.5f, 0.0f,  0.5f,
        -0.5f, 0.0f, -0.5f,   0.5f, 0.0f,  0.5f,  -0.5f, 0.0f,  0.5f,

        // --- LEAVES (Pyramid Top) ---
        // Front
        -2.0f, 5.0f,  2.0f,   2.0f, 5.0f,  2.0f,   0.0f, 14.0f,  0.0f,
        // Right
         2.0f, 5.0f,  2.0f,   2.0f, 5.0f, -2.0f,   0.0f, 14.0f,  0.0f,
        // Back
         2.0f, 5.0f, -2.0f,  -2.0f, 5.0f, -2.0f,   0.0f, 14.0f,  0.0f,
        // Left
        -2.0f, 5.0f, -2.0f,  -2.0f, 5.0f,  2.0f,   0.0f, 14.0f,  0.0f,
    };

    // 2. Colors
    // CHANGED: Size increased from 108 to 126
    GLfloat color_buffer_data[126] = {
        // --- TRUNK WALLS (Brown) ---
        0.4f, 0.2f, 0.0f,  0.4f, 0.2f, 0.0f,  0.4f, 0.2f, 0.0f,
        0.4f, 0.2f, 0.0f,  0.4f, 0.2f, 0.0f,  0.4f, 0.2f, 0.0f,
        0.4f, 0.2f, 0.0f,  0.4f, 0.2f, 0.0f,  0.4f, 0.2f, 0.0f,
        0.4f, 0.2f, 0.0f,  0.4f, 0.2f, 0.0f,  0.4f, 0.2f, 0.0f,
        0.4f, 0.2f, 0.0f,  0.4f, 0.2f, 0.0f,  0.4f, 0.2f, 0.0f,
        0.4f, 0.2f, 0.0f,  0.4f, 0.2f, 0.0f,  0.4f, 0.2f, 0.0f,
        0.4f, 0.2f, 0.0f,  0.4f, 0.2f, 0.0f,  0.4f, 0.2f, 0.0f,
        0.4f, 0.2f, 0.0f,  0.4f, 0.2f, 0.0f,  0.4f, 0.2f, 0.0f,

        // --- NEW: TRUNK BOTTOM COLORS (Brown) ---
        0.4f, 0.2f, 0.0f,  0.4f, 0.2f, 0.0f,  0.4f, 0.2f, 0.0f,
        0.4f, 0.2f, 0.0f,  0.4f, 0.2f, 0.0f,  0.4f, 0.2f, 0.0f,

        // --- LEAVES (Green to White) ---
        // Front
        0.0f, 0.4f, 0.0f,  0.0f, 0.4f, 0.0f,  1.0f, 1.0f, 1.0f,
        // Right
        0.0f, 0.4f, 0.0f,  0.0f, 0.4f, 0.0f,  1.0f, 1.0f, 1.0f,
        // Back
        0.0f, 0.4f, 0.0f,  0.0f, 0.4f, 0.0f,  1.0f, 1.0f, 1.0f,
        // Left
        0.0f, 0.4f, 0.0f,  0.0f, 0.4f, 0.0f,  1.0f, 1.0f, 1.0f,
    };

    GLuint vertexArrayID;
    GLuint vertexBufferID, colorBufferID;
    GLuint programID;
    GLuint mvpMatrixID;

    void initialize(int numTrees, float range) {
        for (int i = 0; i < numTrees; i++) {
            float x = ((float)rand() / RAND_MAX) * (range * 2) - range;
            float z = ((float)rand() / RAND_MAX) * (range * 2) - range;
            if (abs(x) < 5.0f && abs(z) < 5.0f) continue;
            treePositions.push_back(glm::vec3(x, 0.0f, z));
        }

        glGenVertexArrays(1, &vertexArrayID);
        glBindVertexArray(vertexArrayID);

        glGenBuffers(1, &vertexBufferID);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);

        glGenBuffers(1, &colorBufferID);
        glBindBuffer(GL_ARRAY_BUFFER, colorBufferID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data), color_buffer_data, GL_STATIC_DRAW);

        // Make sure the path "lab2/Trees/" exists and contains tree.vert/tree.frag
        programID = LoadShadersFromFile("../lab2/Tree/tree.vert", "../lab2/Tree/tree.frag");
        mvpMatrixID = glGetUniformLocation(programID, "MVP");
    }


    void render(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, glm::vec3 cameraPos) {
        glUseProgram(programID);
        glBindVertexArray(vertexArrayID);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorBufferID);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

        // --- INFINITE FOREST LOGIC ---
        // This matches the range we will set in main (300 range = 600x600 box)
        float range = 300.0f;
        float worldSize = range * 2.0f;

        // Note: removed 'const' so we can update positions
        for (auto& pos : treePositions) {

            // 1. Wrap Logic: If a tree is too far away, wrap it to the other side
            // This keeps the trees always centered around the camera
            float dx = cameraPos.x - pos.x;
            float dz = cameraPos.z - pos.z;

            // Wrap X axis
            if (dx > range) pos.x += worldSize;
            else if (dx < -range) pos.x -= worldSize;

            // Wrap Z axis
            if (dz > range) pos.z += worldSize;
            else if (dz < -range) pos.z -= worldSize;

            // 2. Culling: Only draw if reasonably close (fog distance)
            // We use a slightly smaller distance than the range to hide the "pop-in"
            if (glm::distance(pos, cameraPos) > 300.0f) continue;

            glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), pos);

            // --- UPDATED SCALE LOGIC ---
            // 1. Use sin/cos of position to create a pseudo-random number between 0.0 and 1.0
            // We use a prime number multiplier (0.13f) to avoid repeating patterns on the grid
            float randomVal = (sin(pos.x * 0.13f) * cos(pos.z * 0.17f)) * 0.5f + 0.5f;

            // 2. Base scale is 2.0x, plus up to 4.0x extra based on randomness
            // Result: Trees range from 2.0x to 6.0x their original size
            float s = 2.0f + (randomVal * 4.0f);

            modelMatrix = glm::scale(modelMatrix, glm::vec3(s, s * 1.2f, s));

            glm::mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;

            glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, &mvp[0][0]);

            glDrawArrays(GL_TRIANGLES, 0, 42);
        }

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glBindVertexArray(0);
    }

    void cleanup() {
        glDeleteBuffers(1, &vertexBufferID);
        glDeleteBuffers(1, &colorBufferID);
        glDeleteVertexArrays(1, &vertexArrayID);
        glDeleteProgram(programID);
    }
};