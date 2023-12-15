#pragma once

#define _CRT_SECURE_NO_WARNINGS
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
		* Called before OnImguiRender
		*/

		void Begin();
		/*
		* Called after OnImguiRender
		*/
		void End();
	private:

	};
}
