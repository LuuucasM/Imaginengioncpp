#pragma once

#include "Events/Event.h"

#include <string>

namespace IM {
	class Layer
	{
	public:
		/*
		Constructor for the Layer Class.
		@param std::string name: The name of the layer. Used mostly for debug purposes
		*/
		Layer(const std::string& name = "Layer");
		virtual  ~Layer();

		/*
		* Called when the layer is attached to the layer stack in the LayerManager
		*/
		virtual void OnAttach() {}

		/*
		* Called when the layer is detached to the layer stack in the LayerManager
		*/
		virtual void OnDetach() {}

		/*
		* Called in main loop to update layer's state
		*/
		virtual void OnUpdate(float dt) {}

		/*
		* Get the name of the layer 
		* @return std::string: The name of the layer 
		*/

		virtual void OnImguiRender() {}

		virtual void OnEvent(Event& event) {}

		const std::string& GetName() const { return layer_name; };
	protected:
		std::string layer_name;
	};
}