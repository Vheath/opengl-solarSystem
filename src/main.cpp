
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
void frameStart();

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
bool moveCamera = true;

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

    // configure global opengl state
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    Shader satelliteShader("../src/ShadersGLSL/satelliteShader.vert", "../src/ShadersGLSL/satelliteShader.frag");

    Shader sunShader("../src/ShadersGLSL/sunShader.vert", "../src/ShadersGLSL/sunShader.frag");

    Shader lightingShader("../src/ShadersGLSL/lightShader.vert", "../src/ShadersGLSL/lightShader.frag");

    Shader skyboxShader("../src/ShadersGLSL/skyboxShader.vert", "../src/ShadersGLSL/skyboxShader.frag");

    // load textures (we now use a utility function to keep the code more
    // organized)
    unsigned int diffuseSunMap = loadTexture("../otherFiles/sunmap.png");

    unsigned int diffuseMercuryMap = loadTexture("../otherFiles/mercurymap.jpeg");

    unsigned int diffuseVenusMap = loadTexture("../otherFiles/venusmap.jpg");

    unsigned int diffuseEarthMap = loadTexture("../otherFiles/earthmap.jpg");

    unsigned int diffuseMarsMap = loadTexture("../otherFiles/marsmap.jpg");

    unsigned int diffuseJupiterMap = loadTexture("../otherFiles/jupitermap.jpg");

    unsigned int diffuseSaturnMap = loadTexture("../otherFiles/saturnmap.jpg");

    unsigned int difuseUranusMap = loadTexture("../otherFiles/uranusmap.jpg");

    unsigned int difuseNeptuneMap = loadTexture("../otherFiles/neptunemap.jpg");
    // shader configuration

    std::vector<std::string> textureFaces = {
        "/home/vheath/cppProjects/opengl-solarSystem/Developing/otherFiles/cubemap/right.jpg",
        "/home/vheath/cppProjects/opengl-solarSystem/Developing/otherFiles/cubemap/left.jpg",
        "/home/vheath/cppProjects/opengl-solarSystem/Developing/otherFiles/cubemap/top.jpg",
        "/home/vheath/cppProjects/opengl-solarSystem/Developing/otherFiles/cubemap/bottom.jpg",
        "/home/vheath/cppProjects/opengl-solarSystem/Developing/otherFiles/cubemap/left.jpg",
        "/home/vheath/cppProjects/opengl-solarSystem/Developing/otherFiles/cubemap/right.jpg",
    };
    unsigned int cubeMapID = loadCubeMap(textureFaces);

    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);

    float skyboxVertices[] = {
        // positions
        -1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        -1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f
    };
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    lightingShader.use();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    //-----------------------------
    // Planets Declaration
    float lightPos[3] = { 0.0f, 0.0f, 0.0f };
    float color[3] = { 1.0f, 0.3f, 0.4f };
    Sphere sun { sunShader.ID, 2.0f };
    std::vector<PlanetBody> planetVec;
    // Mercury first
    planetVec.push_back({ { lightingShader.ID, 5.8f, 88, 59 * 24.0f, 0.24f },
        diffuseMercuryMap,
        GL_TEXTURE1 });

    // Venus third
    planetVec.push_back({ { lightingShader.ID, 10.8f, 225, 243 * 24.0f, 0.60f },
        diffuseVenusMap,
        GL_TEXTURE2 });

    // Earth third
    planetVec.push_back({
        { lightingShader.ID, 15, 365, 24.0f, 0.63f },
        diffuseEarthMap,
        GL_TEXTURE3,
    });

    // Mars fourth
    planetVec.push_back({ { lightingShader.ID, 22.8f, 687, 24.8f, 0.34f },
        diffuseMarsMap,
        GL_TEXTURE4 });
    // Deimos and Phobos

    // Jupiter fifth 50(70) 3.21(7.21)
    planetVec.push_back({ { lightingShader.ID, 50.8f, 4380, 9.8f, 3.21f },
        diffuseJupiterMap,
        GL_TEXTURE5 });

    // Saturn sixth
    planetVec.push_back({ { lightingShader.ID, 80.8f, 10950, 10.5f, 3.0f },
        diffuseSaturnMap,
        GL_TEXTURE6 });

    // Uranus seventh (year 30k)
    planetVec.push_back({ { lightingShader.ID, 100.0f, 20002, 17.0f, 1.0f },
        difuseUranusMap,
        GL_TEXTURE7 });

    // Neptune eighth (year 60k)
    planetVec.push_back({ { lightingShader.ID, 130.0f, 24002, 16.0f, 0.95f },
        difuseNeptuneMap,
        GL_TEXTURE8 });

    planetVec[mars].satVec.push_back({ satelliteShader.ID, planetVec[mars].planet.getTranslate(), 1.3f, 1.264f, 30.2f, 0.067f, glm::vec3(0.4f) });
    planetVec[mars].satVec.push_back({ satelliteShader.ID, planetVec[mars].planet.getTranslate(), 0.6f, 0.31f, 7.6f, 0.087f, glm::vec3(0.4f) });

    planetVec[earth].satVec.push_back({ satelliteShader.ID, planetVec[earth].planet.getTranslate(), 1.5f, 27, 27 * 24, 0.087f, glm::vec3(0.4f) });
    //-----------------------------
    //  render loop
    while (!glfwWindowShouldClose(window)) {
        // input
        processInput(window);

        frameStart();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
            (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 300.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);

        // loop for rendering planets and satellites
        for (int i { 0 }; i < planetVec.size(); ++i) {
            lightingShader.use();
            lightingShader.setVec3("light.position", lightPos[0], lightPos[1], lightPos[2]);
            lightingShader.setVec3("viewPos", camera.Position);

            // light properties
            lightingShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
            lightingShader.setVec3("light.diffuse", 0.7f, 0.7f, 0.7f);
            lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

            // material properties
            lightingShader.setFloat("material.shininess", 8.0f);
            // view/projection transformations
            lightingShader.setMat4("projection", projection);
            lightingShader.setMat4("view", view);

            // world transformation
            lightingShader.setMat4("model", model);
            glActiveTexture(planetVec[i].texGL);
            glBindTexture(GL_TEXTURE_2D, planetVec[i].texture);
            lightingShader.setInt("material.diffuse", planetVec[i].texture - 1);
            lightingShader.setInt("material.specular", planetVec[i].texture - 1);
            planetVec[i].planet.draw();

            // shader specifically for rendering satellites, because they're without texture
            satelliteShader.use();
            // passing same projection and view matrix, lightpos viewpos
            satelliteShader.setMat4("projection", projection);
            satelliteShader.setMat4("view", view);

            satelliteShader.setVec3("light.position", lightPos[0], lightPos[1], lightPos[2]);
            satelliteShader.setVec3("viewPos", camera.Position);
            for (int j { 0 }; j < planetVec[i].satVec.size(); ++j) {
                planetVec[i].satVec[j].draw();
            }
        }

        sunShader.use();

        sunShader.setInt("material.diffuse", 1);
        sunShader.setInt("material.specular", 1);
        // bind diffuse map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, diffuseSunMap);

        sunShader.setMat4("projection", projection);
        sunShader.setMat4("view", view);

        sun.draw();

        // draw skybox as last
        glDepthFunc(GL_LEQUAL); // change depth function so depth test passes when values are equal to depth buffer's content
        skyboxShader.use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
        skyboxShader.setMat4("view", view);
        skyboxShader.setMat4("projection", projection);
        // skybox cube
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapID);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default
        ImGui::Begin("Solar system control menu!");

        ImGui::SliderFloat("Time multiplier", &timeMult, 1.0f, 10000000.0f);
        ImGui::SliderFloat3("Light position", &lightPos[0], -1.0f, 1.0f);
        ImGui::ColorEdit3("Color", color);
        ImGui::End();

        lightingShader.setVec3("material.color", color[0], color[1], color[2]);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // de-allocate all resources once they've outlived their purpose:
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteBuffers(1, &skyboxVBO);
    glfwTerminate();
    return 0;
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
