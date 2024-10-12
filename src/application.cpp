#include <gl/glew.h>
// docs.gl is a good doc website!
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Errors.h"
#include "shader.h"
#include "OBJParser.h"
#include "EventHandler.h"
#include "WorldObject.h"
#include "Camera.h"
#include "Skybox.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "TexturedObject.h"
#include "Light.h"
#include "Material.h"

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


void renderObject(GLFWwindow* window, ShaderManager &shaderManager, WorldObject &worldObject,
                 Camera camera, Material material, Light light) {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    auto lightingShader = std::dynamic_pointer_cast<LightingShader>(shaderManager.getShader("lighting_shader"));
    lightingShader->SetUniforms(glm::mat4(1.0f), camera, material, light);

    worldObject.draw();

    GLCall(glfwSwapBuffers(window)); // Swap front and back buffers
    GLCall(glfwPollEvents()); // Poll for and process events
}


static void MainLoop(GLFWwindow* window)
{
    glm::vec3 position = glm::vec3(3, 0, 3); // Camera pos in World Space
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
    shaderManager.loadShader("texture_shader", "res/shaders/textured.shader");
    shaderManager.loadShader<LightingShader>("lighting_shader", "res/shaders/lighting.shader");

    //TextureManager textureManager;
    //textureManager.loadTexture("brickWall", "res/img/brick_wall.jpg");

    WorldObject monkey = WorldObject("res/obj/monkey.obj", shaderManager["mvp_shader"]);
    
    EventHandler eventHandler(window);
    eventHandler.registerObserver(camera);

    double previousTime = glfwGetTime(); // Get the initial time

    // UI ?
    //TexturedObject brickWall(textureManager.getTextureId("brickWall"));
    //brickWall.set_shader(shaderManager.getShader("texture_shader"));

    GLCall(glEnable(GL_DEPTH_TEST));


    ////////////////////////////


    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };
    // first, configure the cube's VAO (and VBO)
    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);

    // vbo
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    // bao
    glBindVertexArray(cubeVAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute, note offset at the end
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glm::vec3 light_pos(1.0f, 0.0f, 1.0f);

    Light light = {
        glm::vec3(0.2f, 0.2f, 0.2f),
        glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec3(0.2f, 0.2f, 0.2f),
        light_pos,
    };

    Material material = {
        glm::vec3(1.0f, 0.5f, 0.31f),
        glm::vec3(1.0f, 0.5f, 0.31f),
        glm::vec3(0.5f, 0.5f, 0.5f),
        32.0f,
    };

    /////////////////////

    float angle = 0;
    float ang_freq = 1;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        double currentTime = glfwGetTime();
        double dt = currentTime - previousTime;
        previousTime = currentTime;

        camera->update(dt);
                
        renderObject(window, shaderManager, monkey, *camera, material, light);

        //glm::mat4 view = camera->getViewMatrix();
        //glm::mat4 projection = camera->getProjectionMatrix();
        //glm::vec3 camPosition = camera->getPosition();

        //auto lightingShader = std::dynamic_pointer_cast<LightingShader>(shaderManager.getShader("cell_lighting_shader"));

        //lightingShader->SetUniforms(glm::mat4(1.0f), *camera, material, light);

        //// render the cube
        //glBindVertexArray(cubeVAO);
        //glDrawArrays(GL_TRIANGLES, 0, 36);

        //// also draw the lamp object
        //glm::mat4 model = glm::translate(glm::mat4(1.0f), light.lightPos);
        //model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
        //shaderManager["mvp_shader"]->Bind();
        //shaderManager["mvp_shader"]->SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);
        //shaderManager["mvp_shader"]->setUniformMat4("u_MVP", projection * view * model);

        //glBindVertexArray(cubeVAO);
        //glDrawArrays(GL_TRIANGLES, 0, 36);

        //angle = (angle + dt * ang_freq);
        //light.lightPos = glm::vec3(glm::cos(angle), 0.0, glm::sin(angle));

    }
}


int main(void)
{
    GLFWwindow* window = InitializeWindow(640, 480, "Hello World");

    MainLoop(window);

    glfwTerminate();
    return 0;
}