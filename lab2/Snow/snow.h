#pragma once
#include <vector>
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <render/shader.h>
#include <cstdlib> // for rand()

struct Particle {
    glm::vec3 position;
    float speed;
    float wobbleOffset; // To make them drift slightly
};

struct Snow {
    std::vector<Particle> particles;
    std::vector<GLfloat> particle_buffer_data; // For GPU upload

    GLuint vertexArrayID;
    GLuint vertexBufferID;
    GLuint programID;

    // Uniform IDs
    GLuint mvpMatrixID;
    GLuint colorID;

    // Settings
    int particleCount = 4000;
    float spawnRange = 50.0f; // How wide the snow area is around the camera
    float spawnHeight = 40.0f; // How high above the camera snow starts

    void initialize() {
        // Load Shaders (Create these files in the next step)
        programID = LoadShadersFromFile("../lab2/Snow/snow.vert", "../lab2/Snow/snow.frag");

        mvpMatrixID = glGetUniformLocation(programID, "MVP");
        colorID = glGetUniformLocation(programID, "snowColor");

        // Initialize particles with random positions
        for (int i = 0; i < particleCount; i++) {
            Particle p;
            // Random X/Z between -range and +range, Y random height
            float x = ((float)rand() / RAND_MAX * 2.0f - 1.0f) * spawnRange;
            float z = ((float)rand() / RAND_MAX * 2.0f - 1.0f) * spawnRange;
            float y = ((float)rand() / RAND_MAX) * spawnHeight;

            p.position = glm::vec3(x, y, z);
            p.speed = 2.0f + ((float)rand() / RAND_MAX) * 3.0f; // Speed between 2 and 5
            p.wobbleOffset = ((float)rand() / RAND_MAX) * 3.14f * 2.0f;

            particles.push_back(p);
        }

        // Setup OpenGL Buffers
        glGenVertexArrays(1, &vertexArrayID);
        glBindVertexArray(vertexArrayID);

        glGenBuffers(1, &vertexBufferID);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
        // We allocate the buffer now, but will fill it every frame in render()
        glBufferData(GL_ARRAY_BUFFER, particleCount * 3 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

        glEnableVertexAttribArray(0); // Layout 0: Position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glBindVertexArray(0);
    }

    void render(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, float deltaTime, glm::vec3 cameraPos) {
        glUseProgram(programID);

        // 1. Update Particle Positions on CPU
        particle_buffer_data.clear();
        particle_buffer_data.reserve(particleCount * 3);

        for (auto& p : particles) {
            // Move Down
            p.position.y -= p.speed * deltaTime;

            // "Wobble" effect (Sine wave on X based on height)
            float drift = sin(p.position.y * 0.5f + p.wobbleOffset) * deltaTime * 2.0f;
            p.position.x += drift;

            float distXZ = glm::distance(glm::vec2(p.position.x, p.position.z), glm::vec2(cameraPos.x, cameraPos.z));

            // CASE 1: Snow hit the ground (Natural cycle)
            // It fell all the way down, so put it back at the top to fall again.
            if (p.position.y < 0.0f) {
                p.position.y = cameraPos.y + spawnHeight;
                p.position.x = cameraPos.x + ((float)rand() / RAND_MAX * 2.0f - 1.0f) * spawnRange;
                p.position.z = cameraPos.z + ((float)rand() / RAND_MAX * 2.0f - 1.0f) * spawnRange;
            }
            // CASE 2: You walked away (Distance check)
            // The snow is too far, so bring it close.
            // IMPORTANT: Pick a RANDOM height so it appears mid-air instantly!
            else if (distXZ > spawnRange) {
                p.position.y = cameraPos.y + ((float)rand() / RAND_MAX) * spawnHeight; // <--- This fixes the waiting
                p.position.x = cameraPos.x + ((float)rand() / RAND_MAX * 2.0f - 1.0f) * spawnRange;
                p.position.z = cameraPos.z + ((float)rand() / RAND_MAX * 2.0f - 1.0f) * spawnRange;
            }

            // Add to buffer
            particle_buffer_data.push_back(p.position.x);
            particle_buffer_data.push_back(p.position.y);
            particle_buffer_data.push_back(p.position.z);
        }

        // 3. Upload updated positions to GPU
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
        // Use glBufferSubData or glBufferData (orphaning) for updates
        glBufferData(GL_ARRAY_BUFFER, particle_buffer_data.size() * sizeof(GLfloat), particle_buffer_data.data(), GL_STREAM_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // 4. Draw
        glm::mat4 mvp = projectionMatrix * viewMatrix;
        glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, &mvp[0][0]);

        // White snow
        glUniform3f(colorID, 1.0f, 1.0f, 1.0f);

        // Important: Enable Point Size adjustment
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