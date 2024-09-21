#include <gl/glew.h>
// docs.gl is a good doc website!
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Errors.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "shader.h"
#include "VertexArray.h"
#include "OBJParser.h"
#include "EventHandler.h"
#include "WorldObject.h"
#include "Camera.h"
#include "Skybox.h"
#include "ShaderManager.h"

#include <glm/glm.hpp>
#include "stb_image.h"



static GLFWwindow* InitializeWindow(int width, int height, const char* title) {
    /* Initialize the library */
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return nullptr;
    }

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable VSync

    /* can call glfwInit after creating valid context */
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return nullptr;
    }

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    return window;
}


static void renderObject(const WorldObject& object, const Camera& camera) {
    // Get the Model, View, and Projection matrices
    glm::mat4 modelMatrix = object.getModelMatrix();
    glm::mat4 viewMatrix = camera.getViewMatrix();
    glm::mat4 projectionMatrix = camera.getProjectionMatrix();

    // Calculate the MVP matrix
    glm::mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;

    // Send the MVP matrix to the shader
    object.shader->Bind();  // Activate the shader
    object.shader->setUniformMat4("u_MVP", mvp);

    object.Draw();
}


static void MainLoop(GLFWwindow* window)
{
    glm::vec3 position = glm::vec3(0, 9, 10); // Camera pos in World Space
    glm::vec3 viewDirection = glm::vec3(0, 0, 0); // and looks at the origin
    glm::vec3 up = glm::vec3(0, 1, 0);  // Head is up (set to 0,-1,0 to look upside-down)
    const float fov_deg = 45.0f;
    const float aspectRatio = (float)640 / (float)480;
    const float nearPlane = 0.1f;
    const float farPlane = 100.0f;
    auto camera = std::make_shared<Camera>(position, viewDirection, up, fov_deg, aspectRatio, nearPlane, farPlane);
    camera->setTarget(glm::vec3(0, 0, 0));

    ShaderManager shaderManager;
    shaderManager.loadShader("mvp_shader", "res/shaders/mvp_shader.shader");
    shaderManager.loadShader("skybox_shader", "res/shaders/skybox.shader");

    // tea pot stuff

    WorldObject teapot = WorldObject("res/obj/teapot.obj", shaderManager["mvp_shader"]);
    
    WorldObject teapot2 = WorldObject("res/obj/teapot.obj", shaderManager["mvp_shader"]);
    teapot2.setPosition(glm::vec3(5, 0, 0));

    shaderManager["mvp_shader"]->Bind();
    shaderManager["mvp_shader"]->SetUniform4f("u_Color", 0.2f, 0.3f, 0.0f, 1.0f);

    // the value we want to set the r channel to and the increment each fraim
    float r = 0.0f;
    float increment = 0.05f;

    EventHandler eventHandler(window);
    eventHandler.registerObserver(camera);

    double previousTime = glfwGetTime(); // Get the initial time

    // skybox stuff

    std::vector<std::string> faces{
        "res/img/skybox/1.jpg",
        "res/img/skybox/2.jpg",
        "res/img/skybox/3.jpg",
        "res/img/skybox/4.jpg",
        "res/img/skybox/5.jpg",
        "res/img/skybox/6.jpg"
    };
    Skybox skybox = Skybox(shaderManager["skybox_shader"]);
    skybox.LoadSkybox(faces);

    GLCall(glEnable(GL_DEPTH_TEST));

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        double currentTime = glfwGetTime();
        double dt = currentTime - previousTime;
        previousTime = currentTime;

        camera->update(dt);

        /* Render here */
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        teapot.Bind();
        shaderManager["mvp_shader"]->SetUniform4f("u_Color", r, 0.0f, 1.0f - r, 1.0f);
        renderObject(teapot, *camera);


        teapot2.Bind();
        shaderManager["mvp_shader"]->SetUniform4f("u_Color", r, 0.0f, r, 1.0f);
        renderObject(teapot2, *camera);

        //skybox.Render(camera->getViewMatrix(), camera->getProjectionMatrix());

        if (r > 1.f)
            increment = -0.05f;
        else if (r < 0.0f)
            increment = 0.05f;

        r += increment;

        GLCall(glfwSwapBuffers(window)); // Swap front and back buffers
        GLCall(glfwPollEvents()); // Poll for and process events
    }
}


int main(void)
{
    GLFWwindow* window = InitializeWindow(640, 480, "Hello World");

    MainLoop(window);

    glfwTerminate();
    return 0;
}