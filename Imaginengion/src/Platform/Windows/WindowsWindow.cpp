#include "impch.h"
#include "WindowsWindow.h"


namespace IM {
	static bool bGLFWInitialized = false;

	Window* Window::Create(const WindowProps& props) { return new WindowsWindow(props); }

	WindowsWindow::WindowsWindow(const WindowProps& props) { Init(props); }

	WindowsWindow::~WindowsWindow() {
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props) {
		Data.Title = props.Title;
		Data.Width = props.Width;
		Data.Height = props.Height;
		Data._Window = this;

		IMAGINE_CORE_INFO("Creating Window %s (%d %d)", props.Title, props.Width, props.Height);

		//checking if we initialized glfw yet
		if (!bGLFWInitialized) {
			int success = glfwInit();
			IMAGINE_CORE_ASSERT(success, "Could not initialize GLFW!");

			bGLFWInitialized = true;
		}

		_Window = glfwCreateWindow((int)props.Width, (int)props.Height, Data.Title.c_str(), nullptr, nullptr);
		IMAGINE_CORE_ASSERT(_Window, "Could not create GLFW window!");

		glfwMakeContextCurrent(_Window);
		glfwSetWindowUserPointer(_Window, &Data);
		SetVSync(true);

		glfwSetWindowSizeCallback(_Window, [](GLFWwindow *window, int width, int height) {
			WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);
			data->Width = width;
			data->Height = height;
			data->_Window->WindowResizeEvent.Broadcast(width, height);
		});
	}

	void WindowsWindow::Shutdown() {
		if (_Window) {
			glfwDestroyWindow(_Window);
		}
	}

	void WindowsWindow::OnUpdate() {
		glfwPollEvents();
		glfwSwapBuffers(_Window);
	}

	void WindowsWindow::SetVSync(bool enabled) {
		if (enabled)
			glfwSwapInterval(VSYNC_ENABLED);
		else
			glfwSwapInterval(VSYNC_DISABLED);

		Data.VSync = enabled;
	}
}