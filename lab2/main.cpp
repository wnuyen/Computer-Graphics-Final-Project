#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> // Added for value_ptr

// --- GLTF Loading Setup ---
// Define these before including tiny_gltf.h
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <render/shader.h>

#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <map>
#include <tinygltf-2.9.3/tiny_gltf.h>

// Macro for offset
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

// --- Texture Loading Helpers ---
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

// --- Project Includes ---
#include <../lab2/Skybox/skybox.h>
#include <../lab2/Ground/ground.h>
#include <../lab2/Snow/snow.h>
#include <../lab2/Tree/tree.h>
#include <../lab2/Person/person.h>
#include <../lab2/Building/building.h>

// --- Constants ---
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;
const unsigned int SHADOW_WIDTH = 2048;
const unsigned int SHADOW_HEIGHT = 2048;

// --- Global Variables ---
static GLFWwindow *window;

// Camera
static glm::vec3 cameraPos   = glm::vec3(0.0f, 10.0f, 150.0f);
static glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
static glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);
static float yaw   = -90.0f;
static float pitch =  0.0f;
static float lastX =  SCR_WIDTH / 2.0f;
static float lastY =  SCR_HEIGHT / 2.0f;
static bool firstMouse = true;

// Timing
static float deltaTime = 0.0f;
static float lastFrame = 0.0f;

// Lighting (Sun)
static glm::vec3 sunPosition = glm::vec3(0.0f, 500.0f, 100.0f);
static glm::vec3 sunColor    = glm::vec3(1.0f, 0.9f, 0.8f);

// Shadow Mapping
GLuint depthMapFBO;
GLuint depthMap;
GLuint depthShaderID;

// --- Function Prototypes ---
static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
static void processInput(GLFWwindow *window);
static void initShadowMap();
static void updateSun(float timeValue);

int main(void)
{
    // 1. Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW." << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Final Project", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to open a GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // 2. Setup Input & Callbacks
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Capture cursor
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    // 3. Initialize GLAD
    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0) {
        std::cerr << "Failed to initialize OpenGL context." << std::endl;
        return -1;
    }

    // 4. Global OpenGL State
    glClearColor(0.2f, 0.2f, 0.25f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // 5. Initialize Scene Objects
    Skybox skybox;
    skybox.initialize();

    Ground ground;
    ground.initialize(glm::vec3(50.0f, 1.0f, 50.0f));

    Snow snow;
    snow.initialize();

    Tree tree;
    tree.initialize(500, 1000.0f);

    // --- Initialize BOT ---
    MyBot bot;
    bot.initialize();

    // 6. Initialize Shadows & Shaders
    initShadowMap();
    depthShaderID = LoadShadersFromFile("../lab2/Shadow/depth.vert", "../lab2/Shadow/depth.frag");

    // 7. Projection Matrix
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);

    // FPS Counters
    int frames = 0;
    float fTime = 0.0f;

    // Bot Animation Time
    float animationTime = 0.0f;

    // --- Main Render Loop ---
    while (!glfwWindowShouldClose(window))
    {
        // A. Time Logic
        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Bot Update
        animationTime += deltaTime * 2.0f; // 2.0x speed
        bot.update(animationTime);

        // B. Input & Updates
        processInput(window);
        updateSun(currentFrame);

        // C. STEP 1: SHADOW PASS
        // ------------------------------------------------
        float near_plane = 1.0f, far_plane = 1000.0f;
        float orthoSize = 150.0f;

        glm::vec3 lightPos = cameraPos + glm::normalize(sunPosition) * 300.0f;

        glm::mat4 lightProjection = glm::ortho(-orthoSize, orthoSize, -orthoSize, orthoSize, near_plane, far_plane);
        glm::mat4 lightView = glm::lookAt(lightPos, cameraPos, glm::vec3(0.0, 1.0, 0.0));
        glm::mat4 lightSpaceMatrix = lightProjection * lightView;

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        glUseProgram(depthShaderID);

        ground.renderShadow(depthShaderID, lightSpaceMatrix);
        tree.renderShadow(depthShaderID, lightSpaceMatrix, cameraPos);

        // Note: The bot is not rendered in the shadow pass here because
        // it requires a shader that supports skinning (bone deformation)
        // to cast accurate shadows. For now, it will only be visible in the scene.

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // D. STEP 2: SCENE RENDER PASS
        // ------------------------------------------------
        int w, h;
        glfwGetFramebufferSize(window, &w, &h);
        glViewport(0, 0, w, h);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        // Render Objects
        ground.render(viewMatrix, projectionMatrix, cameraPos, sunPosition, sunColor, lightSpaceMatrix, depthMap);
        tree.render(viewMatrix, projectionMatrix, cameraPos, sunPosition, sunColor, lightSpaceMatrix, depthMap);

        // Render Skybox
        glm::mat4 skyboxView = glm::mat4(glm::mat3(viewMatrix));
        glCullFace(GL_FRONT);
        skybox.render(skyboxView, projectionMatrix);
        glCullFace(GL_BACK);

        // Render Snow
        snow.render(viewMatrix, projectionMatrix, deltaTime, cameraPos);

        // --- Render BOT ---
        // Scale down and position the bot
        glm::mat4 botModel = glm::mat4(1.0f);
        botModel = glm::translate(botModel, glm::vec3(0.0f, 0.0f, 20.0f)); // Position 20 units forward
        botModel = glm::scale(botModel, glm::vec3(0.25f)); // Scale it down (adjust as needed)

        // Use sun color/pos for lighting the bot
        glm::vec3 botLightColor = sunColor * 1.5f;
        bot.render(viewMatrix, projectionMatrix, botModel, sunPosition, botLightColor);

        // E. FPS Tracking
        frames++;
        fTime += deltaTime;
        if (fTime > 2.0f) {
            std::stringstream stream;
            stream << std::fixed << std::setprecision(2) << "Final Project | FPS: " << (frames / fTime);
            glfwSetWindowTitle(window, stream.str().c_str());
            frames = 0;
            fTime = 0;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    skybox.cleanup();
    ground.cleanup();
    snow.cleanup();
    tree.cleanup();
    bot.cleanup();

    glfwTerminate();
    return 0;
}

// --------------------------------------------------------------------------------
// Helper: Initialize Shadow Map Framebuffer
// --------------------------------------------------------------------------------
static void initShadowMap() {
    glGenFramebuffers(1, &depthMapFBO);
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// --------------------------------------------------------------------------------
// Helper: Update Sun Position and Color based on Time
// --------------------------------------------------------------------------------
static void updateSun(float timeValue) {
    float cycleSpeed = 0.02f;
    float time = timeValue * cycleSpeed;
    float radius = 500.0f;

    // Orbit Logic
    sunPosition.x = sin(time) * radius;
    sunPosition.y = cos(time) * radius;
    sunPosition.z = 450.0f;

    // Color Logic
    if (sunPosition.y > 0.0f) {
        float heightFactor = glm::clamp(sunPosition.y / radius, 0.0f, 1.0f);
        glm::vec3 noonColor = glm::vec3(1.0f, 0.9f, 0.8f);
        glm::vec3 sunsetColor = glm::vec3(1.0f, 0.4f, 0.0f);
        sunColor = glm::mix(sunsetColor, noonColor, heightFactor) * 1.5f;
    } else {
        sunColor = glm::vec3(0.05f, 0.05f, 0.15f);
    }
}

// --------------------------------------------------------------------------------
// Input Processing
// --------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    float cameraSpeed = 100.0f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

    cameraPos.y = glm::clamp(cameraPos.y, 2.0f, 100.0f);
}

// --------------------------------------------------------------------------------
// Mouse Callback
// --------------------------------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = (xpos - lastX) * 0.1f;
    float yoffset = (lastY - ypos) * 0.1f;
    lastX = xpos;
    lastY = ypos;

    yaw += xoffset;
    pitch += yoffset;
    pitch = glm::clamp(pitch, -89.0f, 89.0f);

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

// --------------------------------------------------------------------------------
// Key Callback
// --------------------------------------------------------------------------------
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_ESCAPE)
            glfwSetWindowShouldClose(window, GL_TRUE);

        if (key == GLFW_KEY_R) {
            cameraPos   = glm::vec3(0.0f, 10.0f, 150.0f);
            cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
            yaw = -90.0f;
            pitch = 0.0f;
            firstMouse = true;
        }
    }
}