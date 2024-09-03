

#include <GLFW/glfw3.h>
#include <iostream>
#include "EventHandeler.h"

void EventHandeler::registerEventCallbacks(GLFWwindow* window) {
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
}

// Callback function for key events
void EventHandeler::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);  // Close the window when ESC is pressed
    }

    if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
        std::cout << "Up arrow key pressed." << std::endl;
        // Handle the UP key press event
    }

    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
        std::cout << "Down arrow key pressed." << std::endl;
        // Handle the DOWN key press event
    }

    // Add more key event handling as needed
}

// Callback function for window resize events
void EventHandeler::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// Callback function for mouse button events
void EventHandeler::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        std::cout << "Left mouse button clicked." << std::endl;
        // Handle left mouse button press
    }
}

// Callback function for cursor position events
void EventHandeler::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    std::cout << "Mouse moved to position (" << xpos << ", " << ypos << ")" << std::endl;
    // Handle mouse movement
}