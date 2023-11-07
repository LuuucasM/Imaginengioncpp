#include "impch.h"
#include "LayerManager.h"


namespace IM {
	LayerManager::LayerManager() {

	}
	LayerManager::~LayerManager() {
 
	}

	void LayerManager::OnUpdate() {
		for (auto layer : Layers) {
			layer->OnUpdate();
		}
	}

	void LayerManager::OnImguiRender() {
		for (auto layer : Layers) {
			layer->OnImguiRender();
		}
	}
	void LayerManager::PushLayer(Layer* layer) {
		Layers.emplace(Layers.begin() + LayerInsertIndex, layer);
		++LayerInsertIndex;
	}
	void LayerManager::PushOverlay(Layer* overlay) {
		Layers.emplace_back(overlay);
	}
	void LayerManager::PopLayer(Layer* layer) {
		//check if layer exists on layer stack
		auto it = std::find(Layers.begin(), Layers.begin()+LayerInsertIndex, layer);
		if (it != Layers.begin() + LayerInsertIndex) {
			//note: popping a layer does not destroy the layer object so watch out for dangling pointers
			layer->OnDetach();
			Layers.erase(it);
			--LayerInsertIndex;
		}
	}
	void LayerManager::PopOverlay(Layer* overlay) {
		//check if overlay exists on layer stack
		auto it = std::find(Layers.begin()+LayerInsertIndex, Layers.end(), overlay);
		if (it != Layers.end()) {
			//note: popping an overlay does not destroy the layer object so watch out for dangling pointers
			overlay->OnDetach();
			Layers.erase(it);
		}
	}
}