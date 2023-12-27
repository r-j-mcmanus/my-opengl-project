#include <gl/glew.h>
// docs.gl is a good doc website!
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


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


/* 
* 
* unsigned int type - openGL types are normally unsigned ints so we use this rather than the type to decouple from opengl
*/
static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str(); // pointer to the start of the source string
    // id for the shader
    // number of source codes
    // pointer to pointer to string
    // if length is null, it is assumed the string is null terminated
    glShaderSource(id, 1, &src, nullptr); 
    glCompileShader(id);

    // error handeling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);

        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

/*
We take in the sorce code for the shaders as strings.
*/
static unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    // openGL requires the source code for the shader to compile, and it returns an id back for accssing the function
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader); 

    //two files, need to link them 
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    //we have linked the shaders stored in a program, so we can delete the shader.
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}


int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* can call glfwInit after creating valid context */
    if (glewInit() != GLEW_OK)
        std::cout << "Error!";

    std::cout << glGetString(GL_VERSION) << std::endl;


    float positions[6] = {
        -0.5f,  -0.5f,
         0.0f,   0.5f,
         0.5f,  -0.5f
    };

    unsigned int buffer;
    //make a buffer and bind it for current use
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    //do not need to give data at initalisation, can be done latter neear use
    //GL_STATIC_DRAW as we will not be updating the triangle, but it is drawn a lot
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);
    // index = 0 as first atribute is what we want to use, is the index used in the vertex shader
    // size = 2 as we have 2 floats in the position we want to use
    // type = GL_FLOAT
    // normalize = false as we are flaots 0<x<1
    // stride = 2 as each vertex only has two data points (x pos and y pos, could also have texture / normals ect n the sasme vertex)
    // offset in the vertex to the data we want (sat we wanted normal, use (const void*) 8, for example)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
    // tell open gl that we are allowed to draw this buffer
    glEnableVertexAttribArray(0);

    ShaderProgramSource source = PaseShader("res/shaders/basic.shader");

    std::cout << "VERTEX" << std::endl;
    std::cout << source.VertexSource << std::endl;
    std::cout << "FRAGMENT" << std::endl;
    std::cout << source.FragmentSource << std::endl;

    unsigned int shader = createShader(source.VertexSource, source.FragmentSource);
    glUseProgram(shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // what are we drawing, what index do we start, how many verts ect
        // this will draw the currently bound buffer
        glDrawArrays(GL_TRIANGLES, 0, 3);


        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}