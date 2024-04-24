
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <glad/glad.h>

#include <GL/gl.h>
#include <cmath>

#include "stb_image/stb_image.h"
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "include/camera.h"
#include "include/common.h"
#include "include/planet.h"
#include "include/satellite.h"
#include "include/shader.h"
#include "include/sphere.h"

#include <iostream>
#include <math.h>
#include <numbers>
#include <string>
#include <vector>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadCubeMap(std::vector<std::string> faces);
unsigned int loadTexture(const char* path);
void renderScene(const Shader& shader);
void frameStart();

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
bool moveCamera = true;
glm::vec3 lightPos = { 0.0f, 0.0f, 0.0f };

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

struct PlanetBody {
    Planet planet;
    unsigned int texture;
    unsigned int texGL;
    std::vector<Satellite> satVec;
};
enum PlanetNum {
    mercury = 0,
    venus,
    earth,
    mars,
    jupiter,
    saturn,
    uranus,
    neptune

};

std::vector<PlanetBody> planetVec;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // build and compile shaders
    // -------------------------
    Shader shader("../src/ShadersGLSL/lightShader.vert", "../src/ShadersGLSL/lightShader.frag");
    Shader simpleDepthShader("../src/ShadersGLSL/depthShader.vert", "../src/ShadersGLSL/depthShader.frag", "../src/ShadersGLSL/depthShader.geom");
    Shader sunShader("../src/ShadersGLSL/sunShader.vert", "../src/ShadersGLSL/sunShader.frag");

    unsigned int diffuseSunMap = loadTexture("../otherFiles/sunmap.png");
    unsigned int diffuseMercuryMap = loadTexture("../otherFiles/mercurymap.jpeg");
    unsigned int diffuseVenusMap = loadTexture("../otherFiles/venusmap.jpg");
    unsigned int diffuseEarthMap = loadTexture("../otherFiles/earthmap.jpg");
    unsigned int diffuseMarsMap = loadTexture("../otherFiles/marsmap.jpg");
    unsigned int diffuseJupiterMap = loadTexture("../otherFiles/jupitermap.jpg");
    unsigned int diffuseSaturnMap = loadTexture("../otherFiles/saturnmap.jpg");
    unsigned int difuseUranusMap = loadTexture("../otherFiles/uranusmap.jpg");
    unsigned int difuseNeptuneMap = loadTexture("../otherFiles/neptunemap.jpg");
    // load textures
    // -------------
    unsigned int woodTexture = loadTexture("../otherFiles/earthmap.jpg");
    //-----------------------------
    // Planets Declaration
    // Sphere sun { sunShader.ID, 2.0f };
    // Mercury first
    planetVec.push_back({ { shader.ID, 5.8f, 88, 59 * 24.0f, 0.24f },
        diffuseMercuryMap,
        GL_TEXTURE1 });

    // Venus third
    planetVec.push_back({ { shader.ID, 10.8f, 225, 243 * 24.0f, 0.60f },
        diffuseVenusMap,
        GL_TEXTURE2 });

    // Earth third
    planetVec.push_back({
        { shader.ID, 15, 365, 24.0f, 0.63f },
        diffuseEarthMap,
        GL_TEXTURE3,
    });

    // Mars fourth
    planetVec.push_back({ { shader.ID, 22.8f, 687, 24.8f, 0.34f },
        diffuseMarsMap,
        GL_TEXTURE4 });
    // Deimos and Phobos

    // Jupiter fifth 50(70) 3.21(7.21)
    planetVec.push_back({ { shader.ID, 50.8f, 4380, 9.8f, 3.21f },
        diffuseJupiterMap,
        GL_TEXTURE5 });

    // Saturn sixth
    planetVec.push_back({ { shader.ID, 80.8f, 10950, 10.5f, 3.0f },
        diffuseSaturnMap,
        GL_TEXTURE6 });

    // Uranus seventh (year 30k)
    planetVec.push_back({ { shader.ID, 100.0f, 20002, 17.0f, 1.0f },
        difuseUranusMap,
        GL_TEXTURE7 });

    // Neptune eighth (year 60k)
    planetVec.push_back({ { shader.ID, 130.0f, 24002, 16.0f, 0.95f },
        difuseNeptuneMap,
        GL_TEXTURE8 });

    planetVec[mars].satVec.push_back({ shader.ID, planetVec[mars].planet.getTranslate(), 1.3f, 1.264f, 30.2f, 0.067f, glm::vec3(0.4f) });
    planetVec[mars].satVec.push_back({ shader.ID, planetVec[mars].planet.getTranslate(), 0.6f, 0.31f, 7.6f, 0.087f, glm::vec3(0.4f) });

    planetVec[earth].satVec.push_back({ shader.ID, planetVec[earth].planet.getTranslate(), 1.5f, 27, 27 * 24, 0.087f, glm::vec3(0.4f) });
    // configure depth map FBO
    // -----------------------
    const unsigned int shadowWidth = 1024, shadowHeight = 1024;
    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);
    // create depth cubemap texture
    unsigned int depthCubemap;
    glGenTextures(1, &depthCubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
    for (unsigned int i = 0; i < 6; ++i)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // shader configuration
    // --------------------
    shader.use();
    shader.setInt("diffuseTexture", 0);
    shader.setInt("depthMap", 1);

    Sphere sun { sunShader.ID, 1.0f };
    // lighting info
    // -------------

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // render loop
    // -----------

    // RenderScene();
    //-----------------------------
    //   render loop
    while (!glfwWindowShouldClose(window)) {
        // input
        processInput(window);

        frameStart();
        lightPos.z = static_cast<float>(sin(glfwGetTime() * 0.5) * 3.0);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 0. create depth cubemap transformation matrices
        // -----------------------------------------------
        float near_plane = 1.0f;
        float far_plane = 405.0f;
        glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float)shadowWidth / (float)shadowHeight, near_plane, far_plane);
        std::vector<glm::mat4> shadowTransforms;
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

        // 1. render scene to depth cubemap
        // --------------------------------
        glViewport(0, 0, shadowWidth, shadowHeight);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        simpleDepthShader.use();
        for (unsigned int i = 0; i < 6; ++i)
            simpleDepthShader.setMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
        simpleDepthShader.setFloat("far_plane", far_plane);
        simpleDepthShader.setVec3("lightPos", lightPos);
        renderScene(simpleDepthShader);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // 2. render scene as normal
        // -------------------------
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.use();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        // set lighting uniforms
        shader.setVec3("lightPos", lightPos);
        shader.setVec3("viewPos", camera.Position);
        shader.setInt("shadows", true); // enable/disable shadows by pressing 'SPACE'
        shader.setFloat("far_plane", far_plane);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseSunMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
        renderScene(shader);
        sunShader.use();
         sunShader.setMat4("projection", projection);
         sunShader.setMat4("view", view);
         sunShader.setInt("material.diffuse", diffuseSunMap - 1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseSunMap);
         sun.draw();

        // draw skybox as last
        // glDepthFunc(GL_LEQUAL);
        // skyboxShader.use();
        // view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
        // skyboxShader.setMat4("view", view);
        // skyboxShader.setMat4("projection", projection);
        //// skybox cube
        // glBindVertexArray(skyboxVAO);
        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapID);
        // glDrawArrays(GL_TRIANGLES, 0, 36);
        // glBindVertexArray(0);
        // glDepthFunc(GL_LESS);

        ImGui::Begin("Solar system control menu!");

        ImGui::SliderFloat("Time multiplier", &timeMult, 1.0f, 10000000.0f);
        ImGui::SliderFloat3("Light position", &lightPos[0], -1.0f, 1.0f);
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // de-allocate all resources once they've outlived their purpose:
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // glDeleteVertexArrays(1, &skyboxVAO);
    // glDeleteBuffers(1, &skyboxVBO);
    glfwTerminate();
    return 0;
}

void renderScene(const Shader& shader)
{
    // room cube
    shader.setInt("reverse_normals", 0); // and of course disable it
    //
    for (int i { 1 }; i < planetVec.size(); ++i) {
        shader.use();
        shader.setBool("drawTexture", true);
        planetVec[i].planet.setShaderID(shader.ID);

        glActiveTexture(planetVec[i].texGL);
        glBindTexture(GL_TEXTURE_2D, planetVec[i].texture);
        shader.setInt("diffuseTexture", planetVec[i].texture - 1);
        planetVec[i].planet.draw();

        shader.setBool("drawTexture", false);
        for (int j { 0 }; j < planetVec[i].satVec.size(); ++j) {
            planetVec[i].satVec[j].draw();
        }
    }
}

void frameStart()
{
    // per-frame time logic
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // render
    glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}
// process all input: query GLFW whether relevant keys are pressed/released this
// frame and react accordingly
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    static int lastFrameLeftShift {};

    if ((glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) && lastFrameLeftShift == GLFW_RELEASE) {
        if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            moveCamera = false;
        } else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            moveCamera = true;
        }
    }
    lastFrameLeftShift = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback
// function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    if (moveCamera)
        camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

unsigned int loadCubeMap(std::vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++) {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        } else {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}
// utility function for loading a 2D texture from file
unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
            GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        stbi_image_free(data);
    } else {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
