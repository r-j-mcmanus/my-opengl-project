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

#include <glm/glm.hpp>

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource PaseShader(const std::string& filepath)
{
    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    // Note file should not have a new line at the end
    std::fstream stream(filepath);
    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            ss[(int)type] << line << "\n";
        }
    }
    return { ss[0].str(), ss[1].str() };
}


GLFWwindow* InitializeWindow(int width, int height, const char* title) {
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


void renderObject(const WorldObject& object, const Camera& camera, Shader& shader) {
    // Get the Model, View, and Projection matrices
    glm::mat4 modelMatrix = object.getModelMatrix();
    glm::mat4 viewMatrix = camera.getViewMatrix();
    glm::mat4 projectionMatrix = camera.getProjectionMatrix();

    // Calculate the MVP matrix
    glm::mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;

    // Send the MVP matrix to the shader
    shader.Bind();  // Activate the shader
    shader.setUniformMat4("u_MVP", mvp);

    // Now render the object with OpenGL calls, e.g., glDrawArrays, glDrawElements, etc.
}


void MainLoop(GLFWwindow* window)
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

    ShaderProgramSource source = PaseShader("res/shaders/mvp_shader.shader");

    std::cout << "VERTEX" << std::endl;
    std::cout << source.VertexSource << std::endl;
    std::cout << "FRAGMENT" << std::endl;
    std::cout << source.FragmentSource << std::endl;

    Shader shader = Shader(source.VertexSource, source.FragmentSource);
    WorldObject teapot = WorldObject("res/obj/teapot.obj", shader);

    shader.Bind();
    shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.0f, 1.0f);

    // the value we want to set the r channel to and the increment each fraim
    float r = 0.0f;
    float increment = 0.05f;

    EventHandler eventHandler(window);
    eventHandler.registerObserver(camera);

    double previousTime = glfwGetTime(); // Get the initial time

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        double currentTime = glfwGetTime();
        double dt = currentTime - previousTime;
        previousTime = currentTime;

        camera->update(dt);

        /* Render here */
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        teapot.Bind();

        //which shader will we use
        // change the uniform before we draw the elements
        shader.SetUniform4f("u_Color", r, 0.0f, 1.0f - r, 1.0f);

        renderObject(teapot, *camera, shader);

        teapot.Draw();

        if (r > 1.f)
            increment = -0.05f;
        else if (r < 0.0f)
            increment = 0.05f;

        r += increment;

        GLCall(glfwSwapBuffers(window)); // Swap front and back buffers
        GLCall(glfwPollEvents()); // Poll for and process events

        std::cout << "cam pos" << camera->getPosition()[0] << " " << camera->getPosition()[1] << " " << camera->getPosition()[2] << std::endl;
    }
}


int main(void)
{
    GLFWwindow* window = InitializeWindow(640, 480, "Hello World");

    MainLoop(window);

    glfwTerminate();
    return 0;
}