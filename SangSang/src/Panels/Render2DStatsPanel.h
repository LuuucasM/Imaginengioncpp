#pragma once

#include "Scene/Scene.h"

namespace IM {
	class Render2DStatsPanel
	{
	public:
		Render2DStatsPanel() = default;
		~Render2DStatsPanel() = default;
		Render2DStatsPanel(const WeakPtr<Scene>& scene);
		void SetContext(const WeakPtr<Scene>& scene);
		void OnImGuiRender();
	private:
		WeakPtr<Scene> _Context;
	};
}
