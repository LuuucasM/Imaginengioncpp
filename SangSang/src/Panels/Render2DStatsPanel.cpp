#include "Render2DStatsPanel.h"

#include "Renderer/Renderer.h"

#include <Imgui/imgui.h>

namespace IM {
    Render2DStatsPanel::Render2DStatsPanel(const WeakPtr<Scene>& scene)
    {
        SetContext(scene);
    }
    void Render2DStatsPanel::SetContext(const WeakPtr<Scene>& scene)
    {
        _Context = scene;
    }
    void Render2DStatsPanel::OnImGuiRender()
	{
        ImGui::Begin("Renderer2D Stats");
        auto stats = Renderer::R2D::GetStats();
        ImGui::Text("Renderer2D Stats: ");
        ImGui::Text("DrawCalls: %d", stats.DrawCalls);
        ImGui::Text("Rect Count: %d", stats.RectCount);
        ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
        ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
        if (!_Context.expired()) {
            ImGui::Text("FPS: %f", _Context.lock()->GetFPS());
        }
        ImGui::End();
	}
}

