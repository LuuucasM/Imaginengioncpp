#pragma once

#include "Core/Layer.h"
#include "Events/SystemEvents.h"

namespace IM {
	class ImguiLayer : public Layer
	{
	public:
		ImguiLayer();
		~ImguiLayer() = default;

		/*
		* Called when the layer is attached to the layer stack in the LayerManager
		*/
		void OnAttach() override;

		/*
		* Called when the layer is detached to the layer stack in the LayerManager
		*/
		void OnDetach() override;

		void OnEvent(Event& e) override;

		/*
		* Called before OnImguiRender
		*/
		void Begin();
		/*
		* Called after OnImguiRender
		*/
		void End();

		void SetBlockEvents(bool block) { _bBlockEvents = block; }
		void SetDarkThemeColors();
		
	private:
		bool _bBlockEvents = true;
	};
}
