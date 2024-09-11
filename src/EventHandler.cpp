

#include <GLFW/glfw3.h>
#include <iostream>
#include "EventHandler.h"
#include "camera.h"
#include "glm/glm.hpp"

// todo change to Observer Pattern

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


    if (key == GLFW_KEY_W) {
        EventHandler* handler = static_cast<EventHandler*>(glfwGetWindowUserPointer(window));

        if (handler && handler->camera) {
            if (action == GLFW_PRESS) {
                std::cout << "w down" << std::endl;
                handler->camera->incrementVelocity(glm::vec3(0, 0, 1));
            }
            else if (action == GLFW_RELEASE) {
                std::cout << "w up" << std::endl;
                handler->camera->incrementVelocity(glm::vec3(0, 0, -1));
            }
        }
    }

    if (key == GLFW_KEY_S) {
        EventHandler* handler = static_cast<EventHandler*>(glfwGetWindowUserPointer(window));

        if (handler && handler->camera) {
            if (action == GLFW_PRESS) {
                std::cout << "s down" << std::endl;
                handler->camera->incrementVelocity(glm::vec3(0, 0, -1));
            }
            else if (action == GLFW_RELEASE) {
                std::cout << "s up" << std::endl;
                handler->camera->incrementVelocity(glm::vec3(0, 0, 1));
            }
        }
    }

    if (key == GLFW_KEY_A) {
        EventHandler* handler = static_cast<EventHandler*>(glfwGetWindowUserPointer(window));

        if (handler && handler->camera) {
            if (action == GLFW_PRESS) {
                std::cout << "a down" << std::endl;
                handler->camera->incrementVelocity(glm::vec3(-1, 0, 0));
            }
            else if (action == GLFW_RELEASE) {
                std::cout << "a up" << std::endl;
                handler->camera->incrementVelocity(glm::vec3(1, 0, 0));
            }
        }
    }

    if (key == GLFW_KEY_D) {
        EventHandler* handler = static_cast<EventHandler*>(glfwGetWindowUserPointer(window));

        if (handler && handler->camera) {
            if (action == GLFW_PRESS) {
                std::cout << "d down" << std::endl;
                handler->camera->setVelocity(glm::vec3(1, 0, 0));
            }
            else if (action == GLFW_RELEASE) {
                std::cout << "d up" << std::endl;
                handler->camera->setVelocity(glm::vec3(-1, 0, 0));
            }
        }
    }
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