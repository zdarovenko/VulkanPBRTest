#include "pbr_window.h"

// std
#include <stdexcept>

namespace pbr
{
	PbrWindow::PbrWindow(int w, int h, std::string name) : width{ w }, height{ h }, windowName{ name }
	{
		initWindow();
	}

	PbrWindow::~PbrWindow()
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void PbrWindow::initWindow()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
	}

	int PbrWindow::shouldClose()
	{
		return glfwWindowShouldClose(window);
	}

	void PbrWindow::createWindowSurface(VkInstance& instance, VkSurfaceKHR* surface)
	{
		if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create window surface!");
		}
	}

	void PbrWindow::framebufferResizeCallback(GLFWwindow* window, int width, int height)
	{
		auto pbrWindow = reinterpret_cast<PbrWindow*>(glfwGetWindowUserPointer(window));
		pbrWindow->frameBufferResized = true;
		pbrWindow->width = width;
		pbrWindow->height = height;
	}
}
