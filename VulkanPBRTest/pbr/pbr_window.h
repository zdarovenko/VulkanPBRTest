#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

//std
#include <string>

namespace pbr
{
	class PbrWindow
	{
	public:

		PbrWindow(int, int, std::string);
		~PbrWindow();

		PbrWindow(const PbrWindow&) = delete;
		PbrWindow& operator=(const PbrWindow&) = delete;

		void initWindow();
		int shouldClose();
		void createWindowSurface(VkInstance& instance, VkSurfaceKHR* surface);
		VkExtent2D getExtent() { return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) }; };
		bool wasWindowResized() { return frameBufferResized; }
		void resetWindowResizedFlag() { frameBufferResized = false; }

		GLFWwindow* getGLFWwindow() const { return window; }

	private:
		GLFWwindow* window;

		int width;
		int height;
		bool frameBufferResized = false;

		std::string windowName;

		static void framebufferResizeCallback(GLFWwindow*, int, int);
	};
}
