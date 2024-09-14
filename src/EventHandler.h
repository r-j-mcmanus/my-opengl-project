#pragma once

#include <vector>
#include <memory>

#include <GLFW/glfw3.h>
#include <GLFW/glfw3.h>

#include "EventListenerInterface.h"

class EventHandler {
private:
    std::vector<std::weak_ptr<EventListenerInterface>> observers; // use weak_ptr to avoid dangling pointers

public:
    EventHandler(GLFWwindow* window);

    void registerObserver(std::shared_ptr<EventListenerInterface> observer);

    void unregisterObserver(std::shared_ptr<EventListenerInterface> observer);

    void notifyKeyEvent(int key, int scancode, int action, int mods);

    void notifyMouseEvent(int button, int action, int mods);

    void notifyWindowResize(int width, int height);

    void cleanExpiredObservers();

    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        EventHandler* handler = static_cast<EventHandler*>(glfwGetWindowUserPointer(window));
        handler->notifyKeyEvent(key, scancode, action, mods);
    }

    static void mouseCallback(GLFWwindow* window, int button, int action, int mods) {
        EventHandler* handler = static_cast<EventHandler*>(glfwGetWindowUserPointer(window));
        handler->notifyMouseEvent(button, action, mods);
    }

    static void windowResizeCallback(GLFWwindow* window, int width, int height) {
        EventHandler* handler = static_cast<EventHandler*>(glfwGetWindowUserPointer(window));
        handler->notifyWindowResize(width, height);
    }
};