#include <vector>
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <render/shader.h>

struct Ground {
    glm::vec3 scale;

    // Geometry, Colors, Indices, UVs (Same as before)
    GLfloat vertex_buffer_data[12] = { -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, -1.0f, -1.0f, 0.0f, -1.0f };
    GLfloat color_buffer_data[12] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
    GLuint index_buffer_data[6] = { 0, 1, 2, 0, 2, 3 };
    GLfloat uv_buffer_data[8] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f };

    GLuint vertexArrayID;
    GLuint vertexBufferID, indexBufferID, colorBufferID, uvBufferID;
    GLuint textureID;
    GLuint programID;

    // Uniform IDs
    GLuint mvpMatrixID;
    GLuint modelMatrixID;
    GLuint textureSamplerID;
    GLuint cameraPosID;
    // --- NEW: Light Uniforms ---
    GLuint lightPosID;
    GLuint lightColorID;

    void initialize(glm::vec3 scale) {
        this->scale = scale;
        textureID = LoadTextureTileBox("../lab2/images/snow.jpg"); // Ensure path is correct

        glGenVertexArrays(1, &vertexArrayID);
        glBindVertexArray(vertexArrayID);

        glGenBuffers(1, &vertexBufferID);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);

        glGenBuffers(1, &colorBufferID);
        glBindBuffer(GL_ARRAY_BUFFER, colorBufferID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data), color_buffer_data, GL_STATIC_DRAW);

        glGenBuffers(1, &uvBufferID);
        glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(uv_buffer_data), uv_buffer_data, GL_STATIC_DRAW);

        glGenBuffers(1, &indexBufferID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_buffer_data), index_buffer_data, GL_STATIC_DRAW);

        programID = LoadShadersFromFile("../lab2/Ground/ground.vert", "../lab2/Ground/ground.frag");

        mvpMatrixID = glGetUniformLocation(programID, "MVP");
        modelMatrixID = glGetUniformLocation(programID, "M");
        textureSamplerID = glGetUniformLocation(programID, "textureSampler");
        cameraPosID = glGetUniformLocation(programID, "cameraPosition");

        // --- Get Light Uniform Locations ---
        lightPosID = glGetUniformLocation(programID, "lightPos");
        lightColorID = glGetUniformLocation(programID, "lightColor");
    }

    // --- UPDATED RENDER SIGNATURE ---
    void render(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, glm::vec3 cameraPosition, glm::vec3 sunPos, glm::vec3 sunColor) {
        glUseProgram(programID);
        glBindVertexArray(vertexArrayID);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorBufferID);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glUniform1i(textureSamplerID, 0);

        glUniform3f(cameraPosID, cameraPosition.x, cameraPosition.y, cameraPosition.z);

        // --- Pass Light Data ---
        glUniform3f(lightPosID, sunPos.x, sunPos.y, sunPos.z);
        glUniform3f(lightColorID, sunColor.x, sunColor.y, sunColor.z);

        // Render Loop (Same as before)
        float chunkSize = scale.x * 2.0f;
        int gridX = (int)floor(cameraPosition.x / chunkSize);
        int gridZ = (int)floor(cameraPosition.z / chunkSize);
        int renderRadius = 5;

        for (int x = -renderRadius; x <= renderRadius; x++) {
            for (int z = -renderRadius; z <= renderRadius; z++) {
                float worldX = (gridX + x) * chunkSize;
                float worldZ = (gridZ + z) * chunkSize;
                glm::vec3 chunkPos(worldX, -1.0f, worldZ);

                glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), chunkPos);
                modelMatrix = glm::scale(modelMatrix, scale);
                glm::mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;

                glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, &mvp[0][0]);
                glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &modelMatrix[0][0]);

                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
            }
        }

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
        glBindVertexArray(0);
    }

    void cleanup() {
        glDeleteBuffers(1, &vertexBufferID);
        glDeleteBuffers(1, &colorBufferID);
        glDeleteBuffers(1, &indexBufferID);
        glDeleteBuffers(1, &uvBufferID);
        glDeleteVertexArrays(1, &vertexArrayID);
        glDeleteProgram(programID);
    }
};