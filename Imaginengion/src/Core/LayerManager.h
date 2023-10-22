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
		* Wrappers for begin and end for the layers vector
		*/
		std::vector<Layer*>::iterator begin() { return Layers.begin(); }
		std::vector<Layer*>::iterator end() { return Layers.end(); }
	private:
		std::vector<Layer*> Layers;
		unsigned int LayerInsertIndex = 0;
	};
}
