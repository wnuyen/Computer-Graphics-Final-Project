#pragma once
#include <vector>
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <render/shader.h>

struct Tree {
    std::vector<glm::vec3> treePositions;


    GLfloat vertex_buffer_data[126] = {
        // Trunk Front
        -0.5f, 0.0f, 0.5f, 0.5f, 0.0f, 0.5f, 0.5f, 5.0f, 0.5f,
        -0.5f, 0.0f, 0.5f, 0.5f, 5.0f, 0.5f, -0.5f, 5.0f, 0.5f,
        // Trunk Back
        0.5f, 0.0f, -0.5f, -0.5f, 0.0f, -0.5f, -0.5f, 5.0f, -0.5f,
        0.5f, 0.0f, -0.5f, -0.5f, 5.0f, -0.5f, 0.5f, 5.0f, -0.5f,
        // Trunk Left
        -0.5f, 0.0f, -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 5.0f, 0.5f,
        -0.5f, 0.0f, -0.5f, -0.5f, 5.0f, 0.5f, -0.5f, 5.0f, -0.5f,
        // Trunk Right
        0.5f, 0.0f, 0.5f, 0.5f, 0.0f, -0.5f, 0.5f, 5.0f, -0.5f,
        0.5f, 0.0f, 0.5f, 0.5f, 5.0f, -0.5f, 0.5f, 5.0f, 0.5f,
        // Trunk Bottom
        -0.5f, 0.0f, -0.5f, 0.5f, 0.0f, -0.5f, 0.5f, 0.0f, 0.5f,
        -0.5f, 0.0f, -0.5f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.5f,
        // Leaves Front
        -2.0f, 5.0f, 2.0f, 2.0f, 5.0f, 2.0f, 0.0f, 14.0f, 0.0f,
        // Leaves Right
        2.0f, 5.0f, 2.0f, 2.0f, 5.0f, -2.0f, 0.0f, 14.0f, 0.0f,
        // Leaves Back
        2.0f, 5.0f, -2.0f, -2.0f, 5.0f, -2.0f, 0.0f, 14.0f, 0.0f,
        // Leaves Left
        -2.0f, 5.0f, -2.0f, -2.0f, 5.0f, 2.0f, 0.0f, 14.0f, 0.0f,
    };

    GLfloat color_buffer_data[126] = {
        // (Copy the same color data you had in your original tree.h here)
        // Trunk (Brown) x 8 triangles (Trunk walls)
        0.4f,0.2f,0.0f, 0.4f,0.2f,0.0f, 0.4f,0.2f,0.0f,  0.4f,0.2f,0.0f, 0.4f,0.2f,0.0f, 0.4f,0.2f,0.0f,
        0.4f,0.2f,0.0f, 0.4f,0.2f,0.0f, 0.4f,0.2f,0.0f,  0.4f,0.2f,0.0f, 0.4f,0.2f,0.0f, 0.4f,0.2f,0.0f,
        0.4f,0.2f,0.0f, 0.4f,0.2f,0.0f, 0.4f,0.2f,0.0f,  0.4f,0.2f,0.0f, 0.4f,0.2f,0.0f, 0.4f,0.2f,0.0f,
        0.4f,0.2f,0.0f, 0.4f,0.2f,0.0f, 0.4f,0.2f,0.0f,  0.4f,0.2f,0.0f, 0.4f,0.2f,0.0f, 0.4f,0.2f,0.0f,
        // Trunk Bottom
        0.4f,0.2f,0.0f, 0.4f,0.2f,0.0f, 0.4f,0.2f,0.0f,  0.4f,0.2f,0.0f, 0.4f,0.2f,0.0f, 0.4f,0.2f,0.0f,
        // Leaves (Green/White)
        0.0f,0.4f,0.0f, 0.0f,0.4f,0.0f, 1.0f,1.0f,1.0f,
        0.0f,0.4f,0.0f, 0.0f,0.4f,0.0f, 1.0f,1.0f,1.0f,
        0.0f,0.4f,0.0f, 0.0f,0.4f,0.0f, 1.0f,1.0f,1.0f,
        0.0f,0.4f,0.0f, 0.0f,0.4f,0.0f, 1.0f,1.0f,1.0f,
    };

    // 3. Normals (NEW)
    // Every 3 values correspond to 1 vertex.
    GLfloat normal_buffer_data[126] = {
        // Trunk Front (0, 0, 1)
        0,0,1, 0,0,1, 0,0,1,  0,0,1, 0,0,1, 0,0,1,
        // Trunk Back (0, 0, -1)
        0,0,-1, 0,0,-1, 0,0,-1,  0,0,-1, 0,0,-1, 0,0,-1,
        // Trunk Left (-1, 0, 0)
        -1,0,0, -1,0,0, -1,0,0,  -1,0,0, -1,0,0, -1,0,0,
        // Trunk Right (1, 0, 0)
        1,0,0, 1,0,0, 1,0,0,  1,0,0, 1,0,0, 1,0,0,
        // Trunk Bottom (0, -1, 0)
        0,-1,0, 0,-1,0, 0,-1,0,  0,-1,0, 0,-1,0, 0,-1,0,
        // Leaves Front (Approximate Slant)
        0,0.5,1, 0,0.5,1, 0,0.5,1,
        // Leaves Right
        1,0.5,0, 1,0.5,0, 1,0.5,0,
        // Leaves Back
        0,0.5,-1, 0,0.5,-1, 0,0.5,-1,
        // Leaves Left
        -1,0.5,0, -1,0.5,0, -1,0.5,0
    };

    GLuint vertexArrayID;
    GLuint vertexBufferID, colorBufferID, normalBufferID;
    GLuint programID;
    GLuint mvpMatrixID, modelMatrixID, lightPosID, lightColorID;

    void initialize(int numTrees, float range) {
        // Random generation...
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

        // --- Bind Normal Buffer ---
        glGenBuffers(1, &normalBufferID);
        glBindBuffer(GL_ARRAY_BUFFER, normalBufferID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(normal_buffer_data), normal_buffer_data, GL_STATIC_DRAW);

        programID = LoadShadersFromFile("../lab2/Tree/tree.vert", "../lab2/Tree/tree.frag");

        mvpMatrixID = glGetUniformLocation(programID, "MVP");
        modelMatrixID = glGetUniformLocation(programID, "M");
        lightPosID = glGetUniformLocation(programID, "lightPos");
        lightColorID = glGetUniformLocation(programID, "lightColor");
    }

    void render(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, glm::vec3 cameraPos, glm::vec3 sunPos, glm::vec3 sunColor) {
        glUseProgram(programID);
        glBindVertexArray(vertexArrayID);
        glDisable(GL_CULL_FACE);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorBufferID);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

        // --- Enable Normals (Attribute 2) ---
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, normalBufferID);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

        // Pass Light
        glUniform3f(lightPosID, sunPos.x, sunPos.y, sunPos.z);
        glUniform3f(lightColorID, sunColor.x, sunColor.y, sunColor.z);

        float range = 300.0f;
        float worldSize = range * 2.0f;

        for (auto& pos : treePositions) {
            // Infinite forest logic...
            float dx = cameraPos.x - pos.x;
            float dz = cameraPos.z - pos.z;
            if (dx > range) pos.x += worldSize;
            else if (dx < -range) pos.x -= worldSize;
            if (dz > range) pos.z += worldSize;
            else if (dz < -range) pos.z -= worldSize;

            if (glm::distance(pos, cameraPos) > 300.0f) continue;

            glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), pos);
            float randomVal = (sin(pos.x * 0.13f) * cos(pos.z * 0.17f)) * 0.5f + 0.5f;
            float s = 2.0f + (randomVal * 4.0f);
            modelMatrix = glm::scale(modelMatrix, glm::vec3(s, s * 1.2f, s));

            glm::mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;

            glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, &mvp[0][0]);
            // Pass Model Matrix (Important for normals!)
            glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &modelMatrix[0][0]);

            glDrawArrays(GL_TRIANGLES, 0, 42);
        }

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
        glBindVertexArray(0);
        glEnable(GL_CULL_FACE);
    }

    void cleanup() {
        glDeleteBuffers(1, &vertexBufferID);
        glDeleteBuffers(1, &colorBufferID);
        glDeleteBuffers(1, &normalBufferID);
        glDeleteVertexArrays(1, &vertexArrayID);
        glDeleteProgram(programID);
    }
};