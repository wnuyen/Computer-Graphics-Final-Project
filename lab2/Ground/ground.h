#include <vector>
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <render/shader.h> // Ensure you include shader loader

struct Ground {
	glm::vec3 scale; // Size of ONE chunk

	// 1. Geometry: A simple flat quad on the XZ plane
	GLfloat vertex_buffer_data[12] = {
		-1.0f, 0.0f,  1.0f,
		 1.0f, 0.0f,  1.0f,
		 1.0f, 0.0f, -1.0f,
		-1.0f, 0.0f, -1.0f,
	};

	// 2. Colors
	GLfloat color_buffer_data[12] = {
		1.0f, 1.0f, 1.0f, // White so texture shows clearly
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
	};

	// 3. Indices
	GLuint index_buffer_data[6] = {
		0, 1, 2,
		0, 2, 3,
	};

	// 4. UVs (0 to 1)
	GLfloat uv_buffer_data[8] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
	};

	GLuint vertexArrayID;
	GLuint vertexBufferID, indexBufferID, colorBufferID, uvBufferID;
	GLuint textureID;
	GLuint programID;

	// Shader IDs
	GLuint mvpMatrixID;
	GLuint modelMatrixID;
	GLuint viewMatrixID;
	GLuint textureSamplerID;
	GLuint cameraPosID;

	void initialize(glm::vec3 scale) {
		this->scale = scale;
		textureID = LoadTextureTileBox("../lab2/images/grass.jpg");

		// Vertex Arrays and Buffers setup...
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

		// Load Shaders
		programID = LoadShadersFromFile("../lab2/Ground/ground.vert", "../lab2/Ground/ground.frag");

		// Uniforms
		mvpMatrixID = glGetUniformLocation(programID, "MVP");
		modelMatrixID = glGetUniformLocation(programID, "M");
		textureSamplerID = glGetUniformLocation(programID, "textureSampler");
		cameraPosID = glGetUniformLocation(programID, "cameraPosition");


	}

	void render(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, glm::vec3 cameraPosition) {
		glUseProgram(programID);

		// 1. Bind Buffers
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

		// 2. Bind Texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glUniform1i(textureSamplerID, 0);

		// 3. Send Camera Position for Fog
		glUniform3f(cameraPosID, cameraPosition.x, cameraPosition.y, cameraPosition.z);

		// 4. Render CHUNKS (Grid Loop)
		// We calculate the "Chunk Coordinate" the camera is currently in
		float chunkSize = scale.x * 2.0f; // *2 because plane is -1 to 1
		int gridX = (int)floor(cameraPosition.x / chunkSize);
		int gridZ = (int)floor(cameraPosition.z / chunkSize);

		// Render a 5x5 grid centered on the camera
		int renderRadius = 5;

		for (int x = -renderRadius; x <= renderRadius; x++) {
			for (int z = -renderRadius; z <= renderRadius; z++) {

				// Calculate world position for this chunk
				float worldX = (gridX + x) * chunkSize;
				float worldZ = (gridZ + z) * chunkSize;
				glm::vec3 chunkPos(worldX, -1.0f, worldZ); // -1.0f height

				// Compute Matrices
				glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), chunkPos);
				modelMatrix = glm::scale(modelMatrix, scale);
				glm::mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;

				// Send Matrices
				glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, &mvp[0][0]);
				glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &modelMatrix[0][0]);

				// Draw Quad
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
			}
		}

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
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