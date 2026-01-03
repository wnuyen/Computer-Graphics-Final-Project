#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <render/shader.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <vector>
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <sstream>
#include <iomanip>

static GLFWwindow *window;
static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
static void processInput(GLFWwindow *window);

// --- Camera System Variables ---
// Camera Position & Direction
static glm::vec3 cameraPos   = glm::vec3(0.0f, 10.0f, 150.0f); // Start slightly up and back
static glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
static glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

// Mouse State
static bool firstMouse = true;
static float yaw   = -90.0f;	// Yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right
static float pitch =  0.0f;
static float lastX =  1024.0f / 2.0;
static float lastY =  768.0f / 2.0;

// Timing (for smooth movement)
static float deltaTime = 0.0f;	// Time between current frame and last frame
static float lastFrame = 0.0f;

// -------------------------------

static GLuint LoadTextureTileBox(const char *texture_file_path) {
    int w, h, channels;
    uint8_t* img = stbi_load(texture_file_path, &w, &h, &channels, 3);
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (img) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture " << texture_file_path << std::endl;
    }
    stbi_image_free(img);

    return texture;
}

static GLuint LoadSkyboxTexture(const char *texture_file_path) {
    int w, h, channels;
    stbi_set_flip_vertically_on_load(true);
    uint8_t* img = stbi_load(texture_file_path, &w, &h, &channels, 0);
    stbi_set_flip_vertically_on_load(false);

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (img) {
        GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, img);
    } else {
        std::cout << "Failed to load texture " << texture_file_path << std::endl;
    }
    stbi_image_free(img);

    return textureID;
}

#include <../lab2/Skybox/skybox.h>
#include <../lab2/Ground/ground.h>
#include <../lab2/Snow/snow.h>
#include <../lab2/Tree/tree.h>
#include <../lab2/Building/building.h>


int main(void)
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW." << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1024, 768, "Final Project", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to open a GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // --- INPUT SETUP ---
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide cursor and capture it (FPS style)
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set callbacks
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    // -------------------

    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0)
    {
        std::cerr << "Failed to initialize OpenGL context." << std::endl;
        return -1;
    }

    glClearColor(0.2f, 0.2f, 0.25f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);


    Skybox skybox;
    skybox.initialize();

    Ground ground;
    ground.initialize(glm::vec3(50.0f, 1.0f, 50.0f));

    Snow snow;
    snow.initialize();

    Tree tree;
    tree.initialize(1000, 1000.0f);

    glm::mat4 viewMatrix, projectionMatrix;
    glm::float32 FoV = 45;
    glm::float32 zNear = 0.1f;
    glm::float32 zFar = 1000.0f;
    projectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, zNear, zFar);

    int frames = 0;
    float fTime = 0.0f;

    do
    {
        // Calculate deltaTime
        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Process movement inputs (Arrow Keys)
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update View Matrix (Look from Camera Position, to Position + Front Vector)
        viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        glm::mat4 skyboxViewMatrix = glm::mat4(glm::mat3(viewMatrix));

        glCullFace(GL_FRONT);
        skybox.render(skyboxViewMatrix, projectionMatrix);
        glCullFace(GL_BACK);

        // Render Ground
        ground.render(viewMatrix, projectionMatrix, cameraPos);

        snow.render(viewMatrix, projectionMatrix, deltaTime, cameraPos);

        tree.render(viewMatrix, projectionMatrix, cameraPos);

        // FPS tracking
        // Count number of frames over a few seconds and take average
        frames++;
        fTime += deltaTime;
        if (fTime > 2.0f) {
            float fps = frames / fTime;
            frames = 0;
            fTime = 0;

            std::stringstream stream;
            stream << std::fixed << std::setprecision(2) << "Final Project | Frames per second (FPS): " << fps;
            glfwSetWindowTitle(window, stream.str().c_str());
        }

        glfwSwapBuffers(window);
        glfwPollEvents();

    }
    while (!glfwWindowShouldClose(window));

    skybox.cleanup();
    ground.cleanup();
    snow.cleanup();
    tree.cleanup();

    glfwTerminate();

    return 0;
}

// --------------------------------------------------------------------------------
// Input Processing with Height Constraints
// --------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    // Speed adjusted by deltaTime for consistency across framerates
    float cameraSpeed = 100.0f * deltaTime;

    // Move Forward
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;

    // Move Backward
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;

    // Strafe Left
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

    // Strafe Right
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

    // --- Height Constraints ---
    // Prevent going below ground (assuming ground is at y=0)
    // We keep it slightly above 0 (e.g., 2.0f) so we don't clip into the floor texture.
    if (cameraPos.y < 2.0f) {
        cameraPos.y = 2.0f;
    }

    // Prevent going too high
    if (cameraPos.y > 100.0f) {
        cameraPos.y = 100.0f;
    }
}

// --------------------------------------------------------------------------------
// Mouse Callback for Rotation (Look)
// --------------------------------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // Reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // Constrain pitch so screen doesn't flip
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

// --------------------------------------------------------------------------------
// Key Callback (Handles single events like Quit or Reset)
// --------------------------------------------------------------------------------
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    // Optional: Reset camera position
    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        cameraPos   = glm::vec3(0.0f, 10.0f, 150.0f);
        cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
        yaw = -90.0f;
        pitch = 0.0f;
        firstMouse = true;
    }
}