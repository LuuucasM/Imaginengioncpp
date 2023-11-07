#pragma once

#include "Layer.h"

namespace IM {
	class LayerManager
	{
	public:
		/*
		*Constructor for the LayerManager class.
		*/
		LayerManager();

		/*
		*Deconstructor for the LayerManager class.
		*/
		~LayerManager();

		/*
		*Tells each layer to update
		*/
		void OnUpdate();

		/*
		*Tells each layer to update
		*/
		void OnImguiRender();
		/*
		*Pushes a new layer into the vector of layers
		*/
		void PushLayer(Layer* layer);
		/*
		*Pushes a new overlay into the vector of layers
		*/
		void PushOverlay(Layer* overlay);

		/*
		*Pops a new layer into the vector of layers
		*/
		void PopLayer(Layer* layer);

		/*
		*Pops a new layer into the vector of layers
		*/
		void PopOverlay(Layer* overlay);


		/*
		* Wrappers to get the begin and end for the layers vector
		*/
		std::vector<Layer*>::iterator begin() { return Layers.begin(); }
		std::vector<Layer*>::iterator end() { return Layers.end(); }
	private:
		std::vector<Layer*> Layers;
		//variable to hold where to insert layers
		//overlays are always push_back'd so keep track of where to put layer
		unsigned int LayerInsertIndex = 0;
	};
}
