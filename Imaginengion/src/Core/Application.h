#pragma once

#include "Core.h"

#include "Window.h"
#include "LayerManager.h"
#include "Events/Event.h"
#include "Events/SystemEvents.h"

#include "Imgui/ImguiLayer.h"

#include "Timestep.h"

#include "Log.h"

int main(int argc, char** argv);

namespace IM {

	class Application
	{
	public:
		/*
		*Constructor and deconstructor for Application object
		*/
		Application(const std::string& name = "Imaginengion");
		virtual ~Application();


		/*
		*/
		void OnEvent(Event& e);
		/*
		*Helper function to get the window object for the application
		*@return Window: the main window of the application
		*/
		Window& GetWindow() { 
			IMAGINE_CORE_ASSERT(_Window, "Window not created!")
			return *_Window;
		}
		/*
		* Function to push layer onto LayerManager vector
		*@param layer: The layer to be pushed onto the vector
		*/
		void PushLayer(Layer* layer);
		/*
		* Function to push layer onto LayerManager vector
		*@param overlay: The overlay to be pushed onto the vector
		*/
		void PushOverlay(Layer* overlay);


		/*
		*Helper function to get the application instance. 
		*/
		static Application& Get() {
			IMAGINE_CORE_ASSERT(_Instance, "Application not created!")
			return *_Instance;
		}

		void Close() { _bRunning = false; }

		ImguiLayer* GetImGuiLayer() { return _ImguiLayer; };
	private:
		/*
		*The function that drives the entire application. Where we call update to different components of the application
		*/
		void Run();
		/*
		*Function to close the window when a Close Window Event is triggered
		*/
		bool OnWindowCloseEvent(WindowCloseEvent& e);
		/*
		*Function to resize the viewport when window resize event is triggered
		*/
		bool OnWindowResizeEvent(WindowResizeEvent& e);
	private:
		//bool controlling the main run loop of the application.
		bool _bRunning = true;
		bool _bMinimized = false;
		ScopePtr<Window> _Window;
		ImguiLayer *_ImguiLayer;
		LayerManager _LayerManager;
	
	private:
		static Application* _Instance;
		friend int ::main(int argc, char** argv);

		TimeStep _Timestep;
	};

	//to be defined in client
	Application* CreateApplication();

}

