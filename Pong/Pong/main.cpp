#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GameConstants.h"
#include "ECSCoordinator.h"
#include "Game.h"
#include "Components.h"
#include "ResourceManager.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

Game Pong(W_WIDTH_INIT, W_HEIGHT_INIT);
ECSCoordinator ECSCoord;
std::map<std::string, Shader> ResourceManager::Shaders;

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(W_WIDTH_INIT, W_HEIGHT_INIT, "Pong", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize glad" << std::endl;
	}

	glViewport(0, 0, W_WIDTH_INIT, W_HEIGHT_INIT);

	glfwSetKeyCallback(window, key_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	Pong.Init(window);

	double deltaTime = 0.f;
	double lastFrame = 0.f;

	while (!glfwWindowShouldClose(window)) {

		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//input
		Pong.ProcessInput(deltaTime);

		//update game state
		Pong.Update(deltaTime);

		//draw
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		Pong.Render();

		glfwSwapBuffers(window);
		//no idea what this does yet
		glfwPollEvents();
	}
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			Pong.Keys[key] = true;
		else if (action == GLFW_RELEASE)
			Pong.Keys[key] = false;
	}
}