#pragma once

#include <GLFW/glfw3.h>
#include "Camera.h"

class EventHandler {
public:
	EventHandler(Camera* camera) : camera(camera) {};
	void registerEventCallbacks(GLFWwindow* window);

private:
	Camera* camera;

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
};