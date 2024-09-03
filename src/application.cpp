#include <gl/glew.h>
// docs.gl is a good doc website!
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "shader.h"
#include "VertexArray.h"
#include "OBJParser.h"

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


void MainLoop(GLFWwindow* window)
{

    OBJParser objParser;
    objParser.parse("res/obj/teapot.obj");

    VertexArray vertexArray;

    // make the vertex array object that will link the array buffer
    // we make the vao with the id we have then bind the newly made vao
    unsigned int vao;
    GLCall(glGenVertexArrays(1, &vao));
    GLCall(glBindVertexArray(vao))

    VertexBuffer vb(objParser);

    //we make all the buffers we need and bind to a vertex array at the end
    unsigned int buffer;
    // index = 0 as first atribute is what we want to use, is the index used in the vertex shader
    // size = 3 as we have 2 floats in the position we want to use
    // type = GL_FLOAT
    // normalize = true 
    // stride = 3 as each vertex only has two data points (x pos and y pos, could also have texture / normals ect n the sasme vertex)
    // offset in the vertex to the data we want (sat we wanted normal, use (const void*) 8, for example)
    vertexArray.AddVertexBuffer(vb, 0, 3, GL_FLOAT, GL_TRUE, sizeof(float) * 2, (const void*)0);

    IndexBuffer indexBuffer(objParser);
    vertexArray.SetIndexBuffer(indexBuffer);
    vertexArray.Bind();

    ShaderProgramSource source = PaseShader("res/shaders/basic.shader");

    std::cout << "VERTEX" << std::endl;
    std::cout << source.VertexSource << std::endl;
    std::cout << "FRAGMENT" << std::endl;
    std::cout << source.FragmentSource << std::endl;

    Shader shader = Shader(source.VertexSource, source.FragmentSource);
    shader.Bind();
    shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.0f, 1.0f);

    // the value we want to set the r channel to and the increment each fraim
    float r = 0.0f;
    float increment = 0.05f;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        //which shader will we use
        shader.Bind();
        // change the uniform before we draw the elements
        shader.SetUniform4f("u_Color", r, 0.0f, 1.0f - r, 1.0f);

        vertexArray.Draw();

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