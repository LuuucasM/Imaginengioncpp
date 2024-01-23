#include "EditorLayer.h"

#include <Imgui/imgui.h>

namespace IM {
    EditorLayer::EditorLayer()
        : Layer("MyApp2D"), _CameraController(1280.0f / 720.0f) {
    }

    void EditorLayer::OnAttach()
    {

        IMAGINE_PROFILE_FUNCTION();

        _Texture = Texture2D::Create("assets/textures/Checkerboard.png");
        _TextureZealot = Texture2D::Create("assets/textures/zealot.png");

        _CameraController.SetZoomLevel(5.0f);

        FrameBufferSpecification fbspec;
        fbspec.Width = 1280;
        fbspec.Height = 720;
        _FrameBuffer = FrameBuffer::Create(fbspec);
    }

    void EditorLayer::OnDetach()
    {

        IMAGINE_PROFILE_FUNCTION();


    }

    void EditorLayer::OnUpdate(float dt)
    {
        IMAGINE_PROFILE_FUNCTION();

        if (_bViewportFocus) {
            _CameraController.OnUpdate(dt);
        }
        Renderer::R2D::ResetStats();
        _FrameBuffer->Bind();
        Renderer::SetClearColor({ 0.31f, 0.31f, 0.31f, 1.0f });
        Renderer::Clear();


        //Beginning of the scene where we tell renderer what to do for the scene
        Renderer::R2D::BeginScene(_CameraController.GetCamera());

        Renderer::R2D::DrawRect({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
        Renderer::R2D::DrawRect({ 0.5f, -0.5f }, { 0.9f, 0.8f }, { 0.3f, 0.8f, 0.2f, 1.0f });
        Renderer::R2D::DrawRect({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, _Texture, 10.0f);
        Renderer::R2D::DrawRotatedRect({ 1.0f, -0.5f }, { 2.0f, 2.0f }, -45.0f, _TextureZealot, 1.0f);
        Renderer::R2D::DrawRotatedRect({ 0.0f, -0.0f }, { 0.8f, 0.8f }, 45.0f, { 0.3f, 0.8f, 0.2f, 1.0f });

        Renderer::R2D::EndScene();

        Renderer::R2D::BeginScene(_CameraController.GetCamera());

        for (float y = -5.0f; y < 5.0f; y += 0.5f) {
            for (float x = -5.0f; x < 5.0f; x += 0.5f) {
                glm::vec4 color = { (x + 5.0f) / 10, 0.3, (y + 5.0f) / 10.0f, 1.0f };
                Renderer::R2D::DrawRect({ x, y }, { 0.45f, 0.45f }, color);
            }
        }

        Renderer::R2D::EndScene();
        _FrameBuffer->Unbind();
    }

    void EditorLayer::OnImguiRender()
    {
        IMAGINE_PROFILE_FUNCTION();
        static bool dockingEnabled = true;
        if (dockingEnabled) {
            //ImGuiIO& testio = ImGui::GetIO();
            //IMAGINE_CORE_WARN("VIEWPORTS IS ENABLED: {}", testio.ConfigFlags & ImGuiConfigFlags_ViewportsEnable);
            static bool p_open = true;
            static bool opt_fullscreen = true;
            static bool opt_padding = false;
            static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

            // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
            // because it would be confusing to have two docking targets within each others.
            ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
            if (opt_fullscreen)
            {
                const ImGuiViewport* viewport = ImGui::GetMainViewport();
                ImGui::SetNextWindowPos(viewport->WorkPos);
                ImGui::SetNextWindowSize(viewport->WorkSize);
                ImGui::SetNextWindowViewport(viewport->ID);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
                window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
                window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
            }
            else
            {
                dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
            }

            // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
            // and handle the pass-thru hole, so we ask Begin() to not render a background.
            if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
                window_flags |= ImGuiWindowFlags_NoBackground;

            // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
            // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
            // all active windows docked into it will lose their parent and become undocked.
            // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
            // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::Begin("DockSpace Demo", &p_open, window_flags);
            ImGui::PopStyleVar();

            if (opt_fullscreen)
                ImGui::PopStyleVar(2);

            // Submit the DockSpace
            ImGuiIO& io = ImGui::GetIO();
            if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
            {
                ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
                ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
            }

            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("File"))
                {
                    if (ImGui::MenuItem("Exit")) { Application::Get().Close(); }
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }

            ImGui::Begin("Settings");

            auto stats = Renderer::R2D::GetStats();
            ImGui::Text("Renderer2D Stats: ");
            ImGui::Text("DrawCalls: %d", stats.DrawCalls);
            ImGui::Text("Rect Count: %d", stats.RectCount);
            ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
            ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

            ImGui::ColorEdit3("Square Color", glm::value_ptr(_SquareColor));

            ImGui::End();

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
            ImGui::Begin("Viewport");

            _bViewportFocus = ImGui::IsWindowFocused();
            _bViewportHovered = ImGui::IsWindowHovered();
            Application::Get().GetImGuiLayer()->SetBlockEvents(!_bViewportFocus || !_bViewportHovered);

            ImVec2 viewportSize = ImGui::GetContentRegionAvail();
            if (_ViewportSize != *((glm::vec2*)&viewportSize) && viewportSize.x > 0 && viewportSize.y > 0) {
                _FrameBuffer->Resize((size_t)viewportSize.x, (size_t)viewportSize.y);
                _ViewportSize = { viewportSize.x, viewportSize.y };

                _CameraController.OnResize((size_t)viewportSize.x, (size_t)viewportSize.y);
            }

            uint32_t textureID = _FrameBuffer->GetColorAttachmentID();
            ImGui::Image((void *)textureID, ImVec2{ _ViewportSize.x, _ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

            ImGui::End();
            ImGui::PopStyleVar();

            ImGui::End();
        }
        else {
            ImGui::Begin("Settings");

            auto stats = Renderer::R2D::GetStats();
            ImGui::Text("Renderer2D Stats: ");
            ImGui::Text("DrawCalls: %d", stats.DrawCalls);
            ImGui::Text("Rect Count: %d", stats.RectCount);
            ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
            ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

            ImGui::ColorEdit3("Square Color", glm::value_ptr(_SquareColor));

            uint32_t textureID = _FrameBuffer->GetColorAttachmentID();
            ImGui::Image((void *)textureID, ImVec2{ 720, 480 }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

            ImGui::End();
        }
    }

    void EditorLayer::OnEvent(Event& e)
    {
        _CameraController.OnEvent(e);
    }

}