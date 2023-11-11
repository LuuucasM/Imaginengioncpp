#pragma once

#include "Renderer/RenderContext.h"

struct GLFWwindow;

namespace IM {
	class OpenGLContext : public RenderContext
	{	
	public:
		OpenGLContext(GLFWwindow *window);
		~OpenGLContext();

		void Init() override;
		void SwapBuffers() override;
	private:
		GLFWwindow* _Window;
	};
}


