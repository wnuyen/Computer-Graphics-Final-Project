struct Skybox {
    // Vertex data for a cube
    GLfloat vertex_buffer_data[72] = {
       // Front face (+Z)
       -1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f,
       // Back face (-Z)
       1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,
       // Left face (-X)
       -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f,
       // Right face (+X)
        1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,
       // Top face (+Y)
       -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
       // Bottom face (-Y)
       -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,
    };

    // Index data remains the same
    GLuint index_buffer_data[36] = {
       0, 1, 2, 0, 2, 3, 4, 5, 6, 4, 6, 7, 8, 9, 10, 8, 10, 11,
       12, 13, 14, 12, 14, 15, 16, 17, 18, 16, 18, 19, 20, 21, 22, 20, 22, 23,
    };

    // UV coordinates carefully mapped to your sky.png layout
    // The image seems to be a 4x3 grid of squares.
    GLfloat uv_buffer_data[48] = {
        // Front face (+Z) maps to the second square in the middle row
        0.25f, 0.333f, 0.50f, 0.333f, 0.50f, 0.666f, 0.25f, 0.666f,
        // Back face (-Z) maps to the fourth square in the middle row
        0.75f, 0.333f, 1.00f, 0.333f, 1.00f, 0.666f, 0.75f, 0.666f,
        // Left face (-X) maps to the first square in the middle row
        0.00f, 0.333f, 0.25f, 0.333f, 0.25f, 0.666f, 0.00f, 0.666f,
        // Right face (+X) maps to the third square in the middle row
        0.50f, 0.333f, 0.75f, 0.333f, 0.75f, 0.666f, 0.50f, 0.666f,
        // Top face (+Y) maps to the square in the top row
        0.25f, 0.666f, 0.50f, 0.666f, 0.50f, 1.000f, 0.25f, 1.000f,
        // Bottom face (-Y) maps to the square in the bottom row
        0.25f, 0.000f, 0.50f, 0.000f, 0.50f, 0.333f, 0.25f, 0.333f,
    };

    GLuint vertexArrayID, vertexBufferID, indexBufferID, uvBufferID, textureID;
    GLuint programID;
    GLuint viewMatrixID, projectionMatrixID, textureSamplerID;

    void initialize() {
       glGenVertexArrays(1, &vertexArrayID);
       glBindVertexArray(vertexArrayID);

       glGenBuffers(1, &vertexBufferID);
       glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
       glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);

       glGenBuffers(1, &uvBufferID);
       glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);
       glBufferData(GL_ARRAY_BUFFER, sizeof(uv_buffer_data), uv_buffer_data, GL_STATIC_DRAW);

       glGenBuffers(1, &indexBufferID);
       glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
       glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_buffer_data), index_buffer_data, GL_STATIC_DRAW);

       programID = LoadShadersFromFile("../lab2/Skybox/skybox.vert", "../lab2/Skybox/skybox.frag");

       viewMatrixID = glGetUniformLocation(programID, "view");
       projectionMatrixID = glGetUniformLocation(programID, "projection");
       textureSamplerID = glGetUniformLocation(programID, "skybox");

       textureID = LoadSkyboxTexture("../lab2/images/sky.png");
    }

    void render(glm::mat4 viewMatrix, glm::mat4 projectionMatrix) {
       // Change depth function to pass when depth is less or equal
       // This allows the skybox (drawn at max depth) to be the background
       glDepthFunc(GL_LEQUAL);

       glUseProgram(programID);

       glEnableVertexAttribArray(0); // Vertex positions
       glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
       glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

       glEnableVertexAttribArray(2); // Texture Coordinates
       glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);
       glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

       glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);

       glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, &viewMatrix[0][0]);
       glUniformMatrix4fv(projectionMatrixID, 1, GL_FALSE, &projectionMatrix[0][0]);

       glActiveTexture(GL_TEXTURE0);
       glBindTexture(GL_TEXTURE_2D, textureID);
       glUniform1i(textureSamplerID, 0);

       glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0);

       glDisableVertexAttribArray(0);
       glDisableVertexAttribArray(2);

       // Set depth function back to default
       glDepthFunc(GL_LESS);
    }

    void cleanup() {
       glDeleteBuffers(1, &vertexBufferID);
       glDeleteBuffers(1, &uvBufferID);
       glDeleteBuffers(1, &indexBufferID);
       glDeleteTextures(1, &textureID);
       glDeleteVertexArrays(1, &vertexArrayID);
       glDeleteProgram(programID);
    }
};