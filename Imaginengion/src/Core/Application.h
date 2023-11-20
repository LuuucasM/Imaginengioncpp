#pragma once

#include "Core.h"

#include "Window.h"
#include "LayerManager.h"
#include "Imgui/ImguiLayer.h"

#include "Renderer/VertexArray.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"

#include "Renderer/OrthographicCamera.h"

#include <memory>

//TEMPORARY------------------------
#include "Renderer/Shader.h"

namespace IM {

	class IMAGINE_API Application
	{
	public:
		/*
		*Constructor and deconstructor for Application object
		*/
		Application();
		virtual ~Application();

		/*
		*The function that drives the entire application. Where we call update to different components of the application
		*/
		void Run();

		/*
		*Helper function to get the window object for the application
		*@return Window: the main window of the application
		*/
		inline Window& GetWindow() { 
			IMAGINE_CORE_ASSERT(_Window, "Window not created!")
			return *_Window;
		}
		/*
		*Function to close the window when a Close Window Event is triggered
		*/
		void OnWindowCloseEvent();


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
	private:
		//bool controlling the main run loop of the application.
		bool bRunning = true;

		std::unique_ptr<Window> _Window;
		ImguiLayer *_ImguiLayer;
		LayerManager _LayerManager;
		static Application* _Instance;

		//FOR MAKING A TRIANGLE --------------
		std::shared_ptr<VertexArray> _VertexArray;
		//FOR MAKING A SQUARE
		std::shared_ptr<VertexArray> _SquareVA;
		//TEMPORARY---------------
		std::shared_ptr<Shader> _Shader;
		std::shared_ptr<Shader> _Shader2;
		
		OrthographicCamera _Camera;
	};

	//to be defined in client
	Application* CreateApplication();

}

