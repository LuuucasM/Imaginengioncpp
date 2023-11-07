#pragma once

#include "Core/Layer.h"
namespace IM {
	class IMAGINE_API ImguiLayer : public Layer
	{
	public:
		ImguiLayer();
		~ImguiLayer();

		/*
		* Called when the layer is attached to the layer stack in the LayerManager
		*/
		virtual void OnAttach();

		/*
		* Called when the layer is detached to the layer stack in the LayerManager
		*/
		virtual void OnDetach();

		/*
		* Called When a layer is 
		*/
		void OnImguiRender() override;

		/*
		*
		*/
		void Begin();
		void End();
	private:

	};
}
