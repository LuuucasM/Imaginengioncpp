#include "EditorLayer.h"

#include "ECS/Entity.h"

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

        _ActiveScene = CreateRefPtr<Scene>();

        _SquareEntity = _ActiveScene->CreateEntity("Square");
        _SquareEntity.AddComponent<C_SpriteRenderer>(glm::vec4(0.8f, 0.3f, 0.3f, 1.0f));
        _SquareEntity = _ActiveScene->CreateEntity("BlueSquare");
        _SquareEntity.AddComponent<C_SpriteRenderer>(glm::vec4(0.3f, 0.3f, 0.8f, 1.0f));

        _CameraEntity = _ActiveScene->CreateEntity("Camera");
        _CameraEntity.AddComponent<C_Camera>();

        _CameraEntity2 = _ActiveScene->CreateEntity("Camera2");
        _CameraEntity2.AddComponent<C_Camera>()._bPrimary = false;
        class CameraController : public ScriptClass {
        public:
            void OnCreate() override {
            }
            void OnDestroy() override {

            }
            void OnUpdate(float dt) {
                auto& transform = _Entity.GetComponent<C_Transform>();
                float speed = 5.0f;

                if (Input::IsKeyPressed(Key::A))
                    transform.Translation.x -= speed * dt;
                if (Input::IsKeyPressed(Key::D))
                    transform.Translation.x += speed * dt;
                if (Input::IsKeyPressed(Key::W))
                    transform.Translation.y += speed * dt;
                if (Input::IsKeyPressed(Key::S))
                    transform.Translation.y -= speed * dt;
            }
        };
        _CameraEntity.AddComponent<C_NativeScript>().Bind<CameraController>();
        _CameraEntity2.AddComponent<C_NativeScript>().Bind<CameraController>();

        _SceneHierarchyPanel = CreateRefPtr<SceneHierarchyPanel>(_ActiveScene);
        _PropertiesPanel = CreateRefPtr<PropertiesPanel>(_SceneHierarchyPanel);
    }

    void EditorLayer::OnDetach()
    {

        IMAGINE_PROFILE_FUNCTION();


    }

    void EditorLayer::OnUpdate(float dt)
    {
        IMAGINE_PROFILE_FUNCTION();
        _FPS = 1.0f / dt;
        //resize viewport if needed
        if (IM::FrameBufferSpecification spec = _FrameBuffer->GetSpecification();
            _ViewportSize.x > 0.0f && _ViewportSize.y > 0.0f &&
            (spec.Width != _ViewportSize.x || spec.Height != _ViewportSize.y)) {
            _FrameBuffer->Resize((size_t)_ViewportSize.x, (size_t)_ViewportSize.y);
            _CameraController.OnResize((size_t)_ViewportSize.x, (size_t)_ViewportSize.y);

            _ActiveScene->OnViewportResize((size_t)_ViewportSize.x, (size_t)_ViewportSize.y);
        }

        if (_bViewportFocus) {
            _CameraController.OnUpdate(dt);
        }

        Renderer::R2D::ResetStats();
        _FrameBuffer->Bind();
        Renderer::SetClearColor({ 0.31f, 0.31f, 0.31f, 1.0f });
        Renderer::Clear();

        _ActiveScene->OnUpdate(dt);

        _FrameBuffer->Unbind();
    }

    void EditorLayer::OnImguiRender()
    {
        IMAGINE_PROFILE_FUNCTION();
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

            _SceneHierarchyPanel->OnImGuiRender();
            _PropertiesPanel->OnImGuiRender();

            ImGui::Begin("Settings");

            auto stats = Renderer::R2D::GetStats();
            ImGui::Text("Renderer2D Stats: ");
            ImGui::Text("DrawCalls: %d", stats.DrawCalls);
            ImGui::Text("Rect Count: %d", stats.RectCount);
            ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
            ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
            ImGui::Text("FPS: %f", _FPS);

            if (_SquareEntity) {
                ImGui::Separator();

                auto& name = _SquareEntity.GetComponent<C_Name>().Name;
                ImGui::Text("%s", name.c_str());

                auto& squareColor = _SquareEntity.GetComponent<C_SpriteRenderer>().Color;
                ImGui::ColorEdit3("Square Color", glm::value_ptr(squareColor));

                ImGui::Separator();
            }

            if (ImGui::Checkbox("Camera A", &_PrimaryCamera)) {
                _CameraEntity.GetComponent<C_Camera>()._bPrimary = _PrimaryCamera;
                _CameraEntity2.GetComponent<C_Camera>()._bPrimary = !_PrimaryCamera;
            }

            ImGui::End();

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
            ImGui::Begin("Viewport");

            _bViewportFocus = ImGui::IsWindowFocused();
            _bViewportHovered = ImGui::IsWindowHovered();
            Application::Get().GetImGuiLayer()->SetBlockEvents(!_bViewportFocus || !_bViewportHovered);

            ImVec2 viewportSize = ImGui::GetContentRegionAvail();
            _ViewportSize = { viewportSize.x, viewportSize.y };

            uint32_t textureID = _FrameBuffer->GetColorAttachmentID();
            ImGui::Image((void *)textureID, ImVec2{ _ViewportSize.x, _ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

            ImGui::End();
            ImGui::PopStyleVar();

            ImGui::End();
    }

    void EditorLayer::OnEvent(Event& e)
    {
        _CameraController.OnEvent(e);
    }

}