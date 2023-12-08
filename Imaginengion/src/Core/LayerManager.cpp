#include "impch.h"
#include "LayerManager.h"


namespace IM {
	LayerManager::LayerManager() {

	}
	LayerManager::~LayerManager() {
		for (Layer* layer : _Layers) {
			layer->OnDetach();
			delete layer;
		}
	}

	void LayerManager::OnUpdate(float dt) {
		for (auto layer : _Layers) {
			layer->OnUpdate(dt);
		}
	}

	void LayerManager::OnImguiRender() {
		for (auto layer : _Layers) {
			layer->OnImguiRender();
		}
	}
	void LayerManager::PushLayer(Layer* layer) {
		_Layers.emplace(_Layers.begin() + _LayerInsertIndex, layer);
		++_LayerInsertIndex;
	}
	void LayerManager::PushOverlay(Layer* overlay) {
		_Layers.emplace_back(overlay);
	}
	void LayerManager::PopLayer(Layer* layer) {
		//check if layer exists on layer stack
		auto it = std::find(_Layers.begin(), _Layers.begin()+_LayerInsertIndex, layer);
		if (it != _Layers.begin() + _LayerInsertIndex) {
			//note: popping a layer does not destroy the layer object so watch out for dangling pointers
			layer->OnDetach();
			_Layers.erase(it);
			--_LayerInsertIndex;
		}
	}
	void LayerManager::PopOverlay(Layer* overlay) {
		//check if overlay exists on layer stack
		auto it = std::find(_Layers.begin()+_LayerInsertIndex, _Layers.end(), overlay);
		if (it != _Layers.end()) {
			//note: popping an overlay does not destroy the layer object so watch out for dangling pointers
			overlay->OnDetach();
			_Layers.erase(it);
		}
	}
}