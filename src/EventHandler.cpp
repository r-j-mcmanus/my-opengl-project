

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // For glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>         // For glm::value_ptr

#include "Transformer.h"
#include "EventListenerInterface.h"
#include "EventHandler.h"


// todo change to Observer Pattern
EventHandler::EventHandler(GLFWwindow* window) {
    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseCallback);
    glfwSetWindowSizeCallback(window, windowResizeCallback);

}

void EventHandler::registerObserver(std::shared_ptr<EventListenerInterface> observer) {
    observers.push_back(observer);
}

void EventHandler::unregisterObserver(std::shared_ptr<EventListenerInterface> observer) {
    // to compare the shared ptr with the weak ptr we need to convert the weak to a shared by using lock
    auto condition = [&observer](const std::weak_ptr<EventListenerInterface>& weakObs) {
        return weakObs.lock() == observer;
        };
    // std::remove_if returns a iterator to the nth element of a list of lenth m, where all elements i > n satisfy the condition 
    // (it rearanges the underlying array to satify this)
    auto newEnd = std::remove_if(observers.begin(), observers.end(), condition);
    // remove all elements from newEnd to the end
    observers.erase(newEnd, observers.end()); 
}

void EventHandler::notifyKeyEvent(int key, int scancode, int action, int mods) {
    for (auto observer_wptr_it : observers) {
        if (auto observer_ptr = observer_wptr_it.lock()) {
            observer_ptr->onKeyEvent(key, scancode, action, mods);
        }
    }
}

void EventHandler::notifyMouseEvent(int button, int action, int mods) {
    for (auto observer_wptr_it : observers) {
        if (auto observer_ptr = observer_wptr_it.lock()) {
            observer_ptr->onMouseEvent(button, action, mods);
        }
    }
}

void EventHandler::notifyWindowResize(int width, int height) {
    for (auto observer_wptr_it : observers) {
        if (auto observer_ptr = observer_wptr_it.lock()) {
            observer_ptr->onWindowResize(width, height);
        }
    }
}

void EventHandler::cleanExpiredObservers() {
    // Iterate through the vector and remove expired weak_ptrs
    for (auto it = observers.begin(); it != observers.end(); ) {
        if (it->expired()) {
            it = observers.erase(it);  // Remove expired weak_ptr
        }
        else {
            ++it;
        }
    }
}