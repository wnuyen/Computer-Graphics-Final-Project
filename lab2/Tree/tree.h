#pragma once
#include <vector>
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <render/shader.h>

struct Tree {
    std::vector<glm::vec3> treePositions;

    // --- SETTINGS: CHANGE THESE TO MOVE/RESIZE THE CIRCLE ---
    glm::vec2 clearingCenter = glm::vec2(-80.0f, 50.0f);
    float clearingRadius = 170.0f;
    // -------------------------------------------------------

    // Store the range so Render knows when to wrap
    float worldHalfWidth = 2000.0f;

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
        // Trunk (Brown)
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
        // Leaves Front
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
    GLuint lightSpaceMatrixID;
    GLuint shadowMapID;

    bool isInsideClearing(float x, float z) {
        float dx = x - clearingCenter.x;
        float dz = z - clearingCenter.y;
        float dist = sqrt(dx * dx + dz * dz);
        return dist < clearingRadius;
    }

    void initialize(int numTrees, float range) {
        treePositions.clear();
        this->worldHalfWidth = range; // Store the range!

        for (int i = 0; i < numTrees; i++) {
            float x = ((float)rand() / RAND_MAX) * (range * 2) - range;
            float z = ((float)rand() / RAND_MAX) * (range * 2) - range;

            // Optional: You can check clearing here, but it's more important in render
            // because the trees move/wrap.
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

        glGenBuffers(1, &normalBufferID);
        glBindBuffer(GL_ARRAY_BUFFER, normalBufferID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(normal_buffer_data), normal_buffer_data, GL_STATIC_DRAW);

        programID = LoadShadersFromFile("../lab2/Tree/tree.vert", "../lab2/Tree/tree.frag");

        mvpMatrixID = glGetUniformLocation(programID, "MVP");
        modelMatrixID = glGetUniformLocation(programID, "M");
        lightPosID = glGetUniformLocation(programID, "lightPos");
        lightColorID = glGetUniformLocation(programID, "lightColor");
        lightSpaceMatrixID = glGetUniformLocation(programID, "lightSpaceMatrix");
        shadowMapID = glGetUniformLocation(programID, "shadowMap");
    }

    void render(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, glm::vec3 cameraPos, glm::vec3 sunPos, glm::vec3 sunColor, glm::mat4 lightSpaceMatrix, GLuint depthMapTexture) {
        glUseProgram(programID);
        glBindVertexArray(vertexArrayID);
        glDisable(GL_CULL_FACE);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorBufferID);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, normalBufferID);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMapTexture);
        glUniform1i(shadowMapID, 1);
        glUniformMatrix4fv(lightSpaceMatrixID, 1, GL_FALSE, &lightSpaceMatrix[0][0]);
        glUniform3f(lightPosID, sunPos.x, sunPos.y, sunPos.z);
        glUniform3f(lightColorID, sunColor.x, sunColor.y, sunColor.z);

        // Use the initialized range
        float range = this->worldHalfWidth;
        float worldSize = range * 2.0f;

        for (auto& pos : treePositions) {
            float dx = cameraPos.x - pos.x;
            float dz = cameraPos.z - pos.z;

            // Wrap trees around the camera
            if (dx > range) pos.x += worldSize;
            else if (dx < -range) pos.x -= worldSize;

            if (dz > range) pos.z += worldSize;
            else if (dz < -range) pos.z -= worldSize;

            // Check clearing AFTER moving the tree to its new wrapped position
            if (isInsideClearing(pos.x, pos.z)) continue;

            // Optimization: Don't draw if too far (fog limit)
            if (glm::distance(pos, cameraPos) > 2000.0f) continue;

            glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), pos);
            float randomVal = (sin(pos.x * 0.13f) * cos(pos.z * 0.17f)) * 0.5f + 0.5f;
            float s = 2.0f + (randomVal * 4.0f);
            modelMatrix = glm::scale(modelMatrix, glm::vec3(s, s * 1.2f, s));

            glm::mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;

            glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, &mvp[0][0]);
            glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &modelMatrix[0][0]);

            glDrawArrays(GL_TRIANGLES, 0, 42);
        }

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
        glBindVertexArray(0);
        glEnable(GL_CULL_FACE);
    }

    void renderShadow(GLuint shaderID, const glm::mat4& lightSpaceMatrix, glm::vec3 cameraPos) {
        glBindVertexArray(vertexArrayID);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        GLuint modelLoc = glGetUniformLocation(shaderID, "model");
        GLuint lightSpaceLoc = glGetUniformLocation(shaderID, "lightSpaceMatrix");
        glUniformMatrix4fv(lightSpaceLoc, 1, GL_FALSE, &lightSpaceMatrix[0][0]);

        // Use the initialized range
        float range = this->worldHalfWidth;
        float worldSize = range * 2.0f;

        for (auto& pos : treePositions) {
            // Shadow pass must mirror the render pass wrapping exactly
            glm::vec3 renderPos = pos;

            float dx = cameraPos.x - renderPos.x;
            float dz = cameraPos.z - renderPos.z;

            if (dx > range) renderPos.x += worldSize;
            else if (dx < -range) renderPos.x -= worldSize;

            if (dz > range) renderPos.z += worldSize;
            else if (dz < -range) renderPos.z -= worldSize;

            if (isInsideClearing(renderPos.x, renderPos.z)) continue;
            if (glm::distance(renderPos, cameraPos) > 2000.0f) continue;

            glm::mat4 model = glm::translate(glm::mat4(1.0f), renderPos);
            float randomVal = (sin(renderPos.x * 0.13f) * cos(renderPos.z * 0.17f)) * 0.5f + 0.5f;
            float s = 2.0f + (randomVal * 4.0f);
            model = glm::scale(model, glm::vec3(s, s * 1.2f, s));

            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
            glDrawArrays(GL_TRIANGLES, 0, 42);
        }
        glDisableVertexAttribArray(0);
        glBindVertexArray(0);
    }

    void cleanup() {
        glDeleteBuffers(1, &vertexBufferID);
        glDeleteBuffers(1, &colorBufferID);
        glDeleteBuffers(1, &normalBufferID);
        glDeleteVertexArrays(1, &vertexArrayID);
        glDeleteProgram(programID);
    }
};