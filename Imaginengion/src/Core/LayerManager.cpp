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
	void LayerManager::PushLayer(Layer* layer) {
		Layers.emplace(Layers.begin() + LayerInsertIndex, layer);
		++LayerInsertIndex;
	}
	void LayerManager::PushOverlay(Layer* overlay) {
		Layers.emplace_back(overlay);
	}
	void LayerManager::PopLayer(Layer* layer) {
		auto it = std::find(Layers.begin(), Layers.begin()+LayerInsertIndex, layer);
		if (it != Layers.begin() + LayerInsertIndex) {
			layer->OnDetach();
			Layers.erase(it);
			--LayerInsertIndex;
		}
	}
	void LayerManager::PopOverlay(Layer* overlay) {
		auto it = std::find(Layers.begin()+LayerInsertIndex, Layers.end(), overlay);
		if (it != Layers.end()) {
			overlay->OnDetach();
			Layers.erase(it);
		}
	}
}