#include "impch.h"
#include "WindowsWindow.h"

namespace IM {
	static bool bGLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description) {
		IMAGINE_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	Window* Window::Create(const WindowProps& props) { return new WindowsWindow(props); }

	WindowsWindow::WindowsWindow(const WindowProps& props) { 
		IMAGINE_PROFILE_FUNCTION(); 
		Init(props); 
	}

	WindowsWindow::~WindowsWindow() {
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props) {
		IMAGINE_PROFILE_FUNCTION();
		Data.Title = props.Title;
		Data.Width = props.Width;
		Data.Height = props.Height;
		Data._Window = this;

		IMAGINE_CORE_INFO("Creating Window {0} ({1} {2})", props.Title, props.Width, props.Height);

		//checking if we initialized glfw yet
		if (!bGLFWInitialized) {
			int success = glfwInit();
			IMAGINE_CORE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
			bGLFWInitialized = true;
		}

		//create window with specificed properties
		_Window = glfwCreateWindow((int)props.Width, (int)props.Height, Data.Title.c_str(), nullptr, nullptr);
		IMAGINE_CORE_ASSERT(_Window, "Could not create GLFW window!");

		_RenderContext = CreateScopePtr<OpenGLContext>(_Window);
		_RenderContext->Init();

		//setting window user pointer allows us to pass our class data into glfw callback functions as seen below
		//modify the data struct if more data is needed to properly use glfw callback stuff
		glfwSetWindowUserPointer(_Window, &Data);
		SetVSync(true);

		//set all the glfw callback functions to invoke their associated events defined in the Window class
		glfwSetWindowSizeCallback(_Window, [](GLFWwindow *window, int width, int height) {
			WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);
			data->Width = width;
			data->Height = height;
			data->_Window->WindowResizeEvent.Broadcast(width, height);
		});

		glfwSetWindowCloseCallback(_Window, [](GLFWwindow* window) {
			WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);
			data->_Window->WindowCloseEvent.Broadcast();
		});

		glfwSetKeyCallback(_Window, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
			WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);
			switch (action) {
				case GLFW_PRESS:
				{
					data->_Window->KeyPressedEvent.Broadcast(key, 0);
					break;
				}
				case GLFW_RELEASE:
				{
					data->_Window->KeyReleasedEvent.Broadcast(key);
					break;
				}
				case GLFW_REPEAT:
				{
					data->_Window->KeyRepeatEvent.Broadcast(key, 1);
					break;
				}
			}
		});

		glfwSetMouseButtonCallback(_Window, [](GLFWwindow *window, int button, int action, int mods) {
			WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);

			switch (action) {
				case GLFW_PRESS:
				{
					data->_Window->MouseButtonPressedEvent.Broadcast(button);
					break;
				}
				case GLFW_RELEASE:
				{
					data->_Window->MouseButtonReleasedEvent.Broadcast(button);
					break;
				}
			}
		});

		glfwSetScrollCallback(_Window, [](GLFWwindow *window, double xOffset, double yOffset) {
			WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);

			data->_Window->MouseScrolledEvent.Broadcast((float)xOffset, (float)yOffset);
		});

		glfwSetCursorPosCallback(_Window, [](GLFWwindow *window, double xPos, double yPos) {
			WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);

			data->_Window->MouseMovedEvent.Broadcast((float)xPos, (float)yPos);
		});
	}

	void WindowsWindow::Shutdown() {
		IMAGINE_PROFILE_FUNCTION();
		if (_Window) {
			glfwDestroyWindow(_Window);
		}
	}

	void WindowsWindow::OnUpdate() {
		IMAGINE_PROFILE_FUNCTION();
		glfwPollEvents();
		_RenderContext->SwapBuffers();
	}

	void WindowsWindow::SetVSync(bool enabled) {

		IMAGINE_PROFILE_FUNCTION();

		if (enabled)
			glfwSwapInterval(VSYNC_ENABLED);
		else
			glfwSwapInterval(VSYNC_DISABLED);

		Data.VSync = enabled;
	}
}