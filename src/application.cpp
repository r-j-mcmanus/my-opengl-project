#include <gl/glew.h>
// docs.gl is a good doc website!
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

// macros for error logging openGL
// #x returns the litteral string eg #h = "foo"
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

/*
Clears the GL error buffer
*/
static void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

/*
CleLogs any errors in the GL error buffer
*/
static bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << ")" << std::endl;
        return false;
    }
    return true;
}

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
    GLCall(unsigned int id = glCreateShader(type));
    const char* src = source.c_str(); // pointer to the start of the source string
    // id for the shader
    // number of source codes
    // pointer to pointer to string
    // if length is null, it is assumed the string is null terminated
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    // error handeling
    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE)
    {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));

        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;
        std::cout << message << std::endl;
        GLCall(glDeleteShader(id));
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
    GLCall(unsigned int program = glCreateProgram());
    
    GLCall(unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader));
    GLCall(unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader));

    //two files, need to link them 
    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    //we have linked the shaders stored in a program, so we can delete the shader.
    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

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

    // swap the image 1 time per fraim 
    glfwSwapInterval(1);

    /* can call glfwInit after creating valid context */
    if (glewInit() != GLEW_OK)
        std::cout << "Error!";

    std::cout << glGetString(GL_VERSION) << std::endl;

    //vertex info
    float positions[] = {
        -0.5f,  -0.5f,
         0.5f,  -0.5f,
         0.5f,   0.5f,
        -0.5f,   0.5f
    };

    // index buffer for drawing triangles
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };



    // make the vertex array object that will link the array buffer
    // we make the vao with the id we have then bind the newly made vao
    unsigned int vao;
    GLCall(glGenVertexArrays(1, &vao));
    GLCall(glBindVertexArray(vao))

    //we make all the buffers we need and bind to a vertex array at the end
    unsigned int buffer;
    //make a buffer and bind it for current use
    GLCall(glGenBuffers(1, &buffer));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    //do not need to give data at initalisation, can be done latter neear use
    //GL_STATIC_DRAW as we will not be updating the triangle, but it is drawn a lot
    GLCall(glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions, GL_STATIC_DRAW));
    // index = 0 as first atribute is what we want to use, is the index used in the vertex shader
    // size = 2 as we have 2 floats in the position we want to use
    // type = GL_FLOAT
    // normalize = false as we are flaots 0<x<1
    // stride = 2 as each vertex only has two data points (x pos and y pos, could also have texture / normals ect n the sasme vertex)
    // offset in the vertex to the data we want (sat we wanted normal, use (const void*) 8, for example)
    // this line is what binds the buffer to the vao
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));
    // tell open gl that we are allowed to draw this buffer
    // this allows us to ask to draw the vertexbuffer and index buffer again without spessifying it again
    GLCall(glEnableVertexAttribArray(0));


    unsigned int ibo; // index buffer object, has to be unsigned
    //make a buffer and bind it for current use
    GLCall(glGenBuffers(1, &ibo));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    //do not need to give data at initalisation, can be done latter neear use
    //GL_STATIC_DRAW as we will not be updating the triangle, but it is drawn a lot
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));



    ShaderProgramSource source = PaseShader("res/shaders/basic.shader");

    std::cout << "VERTEX" << std::endl;
    std::cout << source.VertexSource << std::endl;
    std::cout << "FRAGMENT" << std::endl;
    std::cout << source.FragmentSource << std::endl;

    unsigned int shader = createShader(source.VertexSource, source.FragmentSource);
    GLCall(glUseProgram(shader));

    // get the uniform variable from the shader in the form of its id, and then 
    // we can set the value by passing the id and the values we wish to use
    GLCall(int location = glGetUniformLocation(shader, "u_Color"));
    GLCall(glUniform4f(location, 0.2f, 0.3f, 0.0f, 1.0f));

    // the value we want to set the r channel to and the increment each fraim
    float r = 0.0f;
    float increment = 0.05f;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        //which shader will we use
        GLCall(glUseProgram(shader));
        // change the unifrom before we draw the elements using the shader
        GLCall(glUniform4f(location, r, 0.0f, 1.0f - r, 1.0f));

        // bind vao so we dont need to rebind the buffer and vertex pointer
        GLCall(glBindVertexArray(vao));

        // bind the index array to say which elelemnts in the vertex array we want to draw
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));

        // draw ibo as it prevents duplicating vertex info in the gpu memory
        // what are we drawing, what index do we start, how many verts ect
        // this will draw the currently bound buffer
        // has to be unsigned
        // null ptr as we have bound ibo to GL_ELEMENT_ARRAY_BUFFER
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        if (r > 1.f)
            increment = -0.05f;
        else if (r < 0.0f)
            increment = 0.05f;

        r += increment;


        /* Swap front and back buffers */
        GLCall(glfwSwapBuffers(window));

        /* Poll for and process events */
        GLCall(glfwPollEvents());
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}