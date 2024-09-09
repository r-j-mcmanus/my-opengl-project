

#include <GLFW/glfw3.h>
#include <iostream>
#include "EventHandler.h"
#include "camera.h"
#include "glm/glm.hpp"

void EventHandler::registerEventCallbacks(GLFWwindow* window) {
    // Store the pointer to this instanec in the window's user pointer so that it can be used in the callback functions
    glfwSetWindowUserPointer(window, this);


    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
}

// Callback function for key events
void EventHandler::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);  // Close the window when ESC is pressed
    }

    if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
        std::cout << "Up arrow key pressed." << std::endl;
        // Handle the UP key press event
    }

    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
        EventHandler* handler = static_cast<EventHandler*>(glfwGetWindowUserPointer(window));

        if (handler && handler->camera) {
            handler->camera->translate(glm::vec3(0,0,1));
        }
    }

    if (key == GLFW_KEY_S && action == GLFW_PRESS) {
        EventHandler* handler = static_cast<EventHandler*>(glfwGetWindowUserPointer(window));

        if (handler && handler->camera) {
            handler->camera->translate(glm::vec3(0, 0, -1));
        }
    }

    if (key == GLFW_KEY_A && action == GLFW_PRESS) {
        EventHandler* handler = static_cast<EventHandler*>(glfwGetWindowUserPointer(window));

        if (handler && handler->camera) {
            handler->camera->translate(glm::vec3(-1, 0, 0));
        }
    }

    if (key == GLFW_KEY_D && action == GLFW_PRESS) {
        EventHandler* handler = static_cast<EventHandler*>(glfwGetWindowUserPointer(window));

        if (handler && handler->camera) {
            handler->camera->translate(glm::vec3(1, 0, 0));
        }
    }

    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
        std::cout << "Down arrow key pressed." << std::endl;
        // Handle the DOWN key press event
    }

    // Add more key event handling as needed
}

// Callback function for window resize events
void EventHandler::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // get back the 'this' pointer and cast it back to EventHandler object so we can use the camera pointer the class holds
    // we do this so that this object can ue all interfaces in the callbacks
    EventHandler* handler = static_cast<EventHandler*>(glfwGetWindowUserPointer(window));

    // Ensure the camera is valid before calling setFov
    if (handler && handler->camera) {
        handler->camera->setAspectRatio(width, height);
    }

    glViewport(0, 0, width, height);
}

// Callback function for mouse button events
void EventHandler::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        std::cout << "Left mouse button clicked." << std::endl;
        // Handle left mouse button press
    }
}

// Callback function for cursor position events
void EventHandler::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    std::cout << "Mouse moved to position (" << xpos << ", " << ypos << ")" << std::endl;
    // Handle mouse movement
}