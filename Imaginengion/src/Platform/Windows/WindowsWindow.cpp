#include "impch.h"
#include "WindowsWindow.h"

#include "Events/SystemEvents.h"

#include "Renderer/RendererAPI.h"

namespace IM {

	static uint8_t _WindowCount = 0;

	static void GLFWErrorCallback(int error, const char* description) {
		IMAGINE_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	ScopePtr<Window> Window::Create(const WindowProps& props) { 
		return CreateScopePtr<WindowsWindow>(props); 
	}

	WindowsWindow::WindowsWindow(const WindowProps& props) { 
		IMAGINE_PROFILE_FUNCTION(); 
		Init(props); 
	}

	WindowsWindow::~WindowsWindow() {
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props) {
		IMAGINE_PROFILE_FUNCTION();
		_Data.Title = props.Title;
		_Data.Width = props.Width;
		_Data.Height = props.Height;

		IMAGINE_CORE_INFO("Creating Window {0} ({1} {2})", props.Title, props.Width, props.Height);

		//checking if we initialized glfw yet
		if (_WindowCount == 0) {
			int success = glfwInit();
			IMAGINE_CORE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
		}
		#ifdef IMAGINE_DEBUG
			if (RendererAPI::GetCurrentAPI() == RendererAPI::API::OpenGL) {
				glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
			}
		#endif
		//create window with specificed properties
		_Window = glfwCreateWindow((int)props.Width, (int)props.Height, _Data.Title.c_str(), nullptr, nullptr);
		IMAGINE_CORE_ASSERT(_Window, "Could not create GLFW window!");
		++_WindowCount;

		_RenderContext = RenderContext::Create(_Window);
		_RenderContext->Init();

		//setting window user pointer allows us to pass our class data into glfw callback functions as seen below
		//modify the data struct if more data is needed to properly use glfw callback stuff
		glfwSetWindowUserPointer(_Window, &_Data);
		SetVSync(true);

		//set all the glfw callback functions to invoke their associated events defined in the Window class
		glfwSetWindowSizeCallback(_Window, [](GLFWwindow *window, int width, int height) {
			WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);
			data->Width = width;
			data->Height = height;
			WindowResizeEvent event(width, height);
			data->EventCallback(event);
		});

		glfwSetWindowCloseCallback(_Window, [](GLFWwindow* window) {
			WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data->EventCallback(event);
		});

		glfwSetKeyCallback(_Window, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
			WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);
			switch (action) {
				case GLFW_PRESS:
				{
					KeyPressedEvent event((KeyCode)key, 0);
					data->EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event((KeyCode)key);
					data->EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event((KeyCode)key, 1);
					data->EventCallback(event);
					break;
				}
			}
		});

		glfwSetMouseButtonCallback(_Window, [](GLFWwindow *window, int button, int action, int mods) {
			WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);

			switch (action) {
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data->EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data->EventCallback(event);
					break;
				}
			}
		});

		glfwSetScrollCallback(_Window, [](GLFWwindow *window, double xOffset, double yOffset) {
			WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data->EventCallback(event);
		});

		glfwSetCursorPosCallback(_Window, [](GLFWwindow *window, double xPos, double yPos) {
			WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xPos, (float)yPos);
			data->EventCallback(event);
		});
	}

	void WindowsWindow::Shutdown() {

		IMAGINE_PROFILE_FUNCTION();
		if (_Window) {
			glfwDestroyWindow(_Window);
			--_WindowCount;
			if (_WindowCount == 0) {
				glfwTerminate();
			}
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

		_Data.VSync = enabled;
	}
}