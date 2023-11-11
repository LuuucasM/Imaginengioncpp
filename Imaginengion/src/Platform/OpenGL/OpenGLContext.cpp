#include "impch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace IM {
	OpenGLContext::OpenGLContext(GLFWwindow* window)
		: _Window(window){
		IMAGINE_CORE_ASSERT(window, "Window is null when initializing OpenGLContext!")
	}

	OpenGLContext::~OpenGLContext() {}
	void OpenGLContext::Init() {

		glfwMakeContextCurrent(_Window);

		//init glad
		int success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		IMAGINE_CORE_ASSERT(success, "Failed to initialize Glad!");
	}

	void OpenGLContext::SwapBuffers() {
		glfwSwapBuffers(_Window);
	}
}