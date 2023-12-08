#include "impch.h"
#include "OpenGLContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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

		#ifdef IMAGINE_ENABLE_ASSERTS
			IMAGINE_CORE_INFO("OpenGL Info: ");
			IMAGINE_CORE_INFO("    Vendor: {0}", (const char*)glGetString(GL_VENDOR));
			IMAGINE_CORE_INFO("    Renderer: {0}", (const char*)glGetString(GL_RENDERER));
			IMAGINE_CORE_INFO("    Version: {0}", (const char*)glGetString(GL_VERSION));
	
			IMAGINE_CORE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "Imaginengion requires at least OpenGL version 4.5!");
		#endif	
	}

	void OpenGLContext::SwapBuffers() {
		glfwSwapBuffers(_Window);
	}
}