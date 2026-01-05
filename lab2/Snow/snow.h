#pragma once
#include <vector>
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <render/shader.h>
#include <cstdlib>

struct Particle {
    glm::vec3 position;
    float speed;
    float wobbleOffset;
};

struct Snow {
    std::vector<Particle> particles;
    std::vector<GLfloat> particle_buffer_data;

    GLuint vertexArrayID;
    GLuint vertexBufferID;
    GLuint programID;

    // Uniform IDs
    GLuint mvpMatrixID;
    GLuint colorID;
    GLuint lightPosID;
    GLuint lightColorID;
    GLuint lightSpaceMatrixID;
    GLuint shadowMapID;

    int particleCount = 4000;
    float spawnRange = 50.0f;
    float spawnHeight = 40.0f;

    void initialize() {
        programID = LoadShadersFromFile("../lab2/Snow/snow.vert", "../lab2/Snow/snow.frag");

        mvpMatrixID = glGetUniformLocation(programID, "MVP");
        colorID     = glGetUniformLocation(programID, "snowColor");

        lightPosID         = glGetUniformLocation(programID, "lightPos");
        lightColorID       = glGetUniformLocation(programID, "lightColor");
        lightSpaceMatrixID = glGetUniformLocation(programID, "lightSpaceMatrix");
        shadowMapID        = glGetUniformLocation(programID, "shadowMap");

        // Initialize particles
        for (int i = 0; i < particleCount; i++) {
            Particle p;
            float x = ((float)rand() / RAND_MAX * 2.0f - 1.0f) * spawnRange;
            float z = ((float)rand() / RAND_MAX * 2.0f - 1.0f) * spawnRange;
            float y = ((float)rand() / RAND_MAX) * spawnHeight;

            p.position = glm::vec3(x, y, z);
            p.speed = 2.0f + ((float)rand() / RAND_MAX) * 3.0f;
            p.wobbleOffset = ((float)rand() / RAND_MAX) * 3.14f * 2.0f;

            particles.push_back(p);
        }

        glGenVertexArrays(1, &vertexArrayID);
        glBindVertexArray(vertexArrayID);

        glGenBuffers(1, &vertexBufferID);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
        glBufferData(GL_ARRAY_BUFFER, particleCount * 3 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glBindVertexArray(0);
    }

    void render(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, float deltaTime, glm::vec3 cameraPos,
                glm::vec3 sunPos, glm::vec3 sunColor, glm::mat4 lightSpaceMatrix, GLuint depthMapTexture) {

        glUseProgram(programID);

        // 1. Update Particles (CPU)
        particle_buffer_data.clear();
        particle_buffer_data.reserve(particleCount * 3);

        for (auto& p : particles) {
            p.position.y -= p.speed * deltaTime;
            float drift = sin(p.position.y * 0.5f + p.wobbleOffset) * deltaTime * 2.0f;
            p.position.x += drift;

            float distXZ = glm::distance(glm::vec2(p.position.x, p.position.z), glm::vec2(cameraPos.x, cameraPos.z));

            if (p.position.y < 0.0f) {
                p.position.y = cameraPos.y + spawnHeight;
                p.position.x = cameraPos.x + ((float)rand() / RAND_MAX * 2.0f - 1.0f) * spawnRange;
                p.position.z = cameraPos.z + ((float)rand() / RAND_MAX * 2.0f - 1.0f) * spawnRange;
            }
            else if (distXZ > spawnRange) {
                p.position.y = cameraPos.y + ((float)rand() / RAND_MAX) * spawnHeight;
                p.position.x = cameraPos.x + ((float)rand() / RAND_MAX * 2.0f - 1.0f) * spawnRange;
                p.position.z = cameraPos.z + ((float)rand() / RAND_MAX * 2.0f - 1.0f) * spawnRange;
            }

            particle_buffer_data.push_back(p.position.x);
            particle_buffer_data.push_back(p.position.y);
            particle_buffer_data.push_back(p.position.z);
        }

        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
        glBufferData(GL_ARRAY_BUFFER, particle_buffer_data.size() * sizeof(GLfloat), particle_buffer_data.data(), GL_STREAM_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // 2. Set Uniforms
        glm::mat4 mvp = projectionMatrix * viewMatrix;
        glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, &mvp[0][0]);
        glUniform3f(colorID, 1.0f, 1.0f, 1.0f);

        glUniform3f(lightPosID, sunPos.x, sunPos.y, sunPos.z);
        glUniform3f(lightColorID, sunColor.x, sunColor.y, sunColor.z);
        glUniformMatrix4fv(lightSpaceMatrixID, 1, GL_FALSE, &lightSpaceMatrix[0][0]);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthMapTexture);
        glUniform1i(shadowMapID, 0);

        // 3. Draw
        glEnable(GL_PROGRAM_POINT_SIZE);
        glBindVertexArray(vertexArrayID);
        glDrawArrays(GL_POINTS, 0, particleCount);

        glBindVertexArray(0);
        glDisable(GL_PROGRAM_POINT_SIZE);
    }

    void cleanup() {
        glDeleteBuffers(1, &vertexBufferID);
        glDeleteVertexArrays(1, &vertexArrayID);
        glDeleteProgram(programID);
    }
};