#include "EditorLayer.h"

#include "ECS/Entity.h"

#include <Imgui/imgui.h>

#include "glm/gtc/type_ptr.hpp"

#include "Scene/SceneSerializer.h"

#include "Utils/PlatformUtils.h"

#include <filesystem>

#include <ImGuizmo/ImGuizmo.h>

#include "glm/gtx/matrix_decompose.hpp"

#include "Math/Math.h"

namespace IM {

    //TODO: when to have a projects object which contains the main working folder relative to the project, then change this
    static std::filesystem::path _AssetDirectory = "assets";

    EditorLayer::EditorLayer()
        : Layer("MyApp2D"){
    }

    void EditorLayer::OnAttach()
    {

        IMAGINE_PROFILE_FUNCTION();

        FrameBufferSpecification fbspec;
        fbspec._Attachments = { FrameBufferTextureFormat::RGBA8, FrameBufferTextureFormat::RED_INTEGER, FrameBufferTextureFormat::Depth };
        fbspec.Width = 1280;
        fbspec.Height = 720;
        _FrameBuffer = FrameBuffer::Create(fbspec);

        _ActiveScene = CreateRefPtr<Scene>();

        _EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

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

        _SceneHierarchyPanel = CreateRefPtr<SceneHierarchyPanel>(_ActiveScene);
        _PropertiesPanel = CreateRefPtr<PropertiesPanel>(_SceneHierarchyPanel);
        _ContentBrowserPanel = CreateRefPtr<ContentBrowserPanel>();
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
            _EditorCamera.SetViewportSize(_ViewportSize.x, _ViewportSize.y);
            _ActiveScene->OnViewportResize((size_t)_ViewportSize.x, (size_t)_ViewportSize.y);
        }

        if (_bViewportFocus) {
            _EditorCamera.OnUpdate(dt);
        }

        

        Renderer::R2D::ResetStats();
        _FrameBuffer->Bind();
        Renderer::SetClearColor({ 0.31f, 0.31f, 0.31f, 1.0f });
        Renderer::Clear();

        _FrameBuffer->ClearColorAttachment(1, 0);

        _ActiveScene->OnUpdateEditor(dt, _EditorCamera);
        
        auto [mx, my] = ImGui::GetMousePos();
        mx -= _ViewportBounds[0].x;
        my -= _ViewportBounds[0].y;
        glm::vec2 viewportSize = _ViewportBounds[1] - _ViewportBounds[0];
        my = viewportSize.y - my;

        int mouseX = (int)mx;
        int mouseY = (int)my;

        if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y) {
            uint32_t pixelData = _FrameBuffer->ReadPixel(1, mouseX, mouseY);
            _HoveredEntity = pixelData == 0 ? Entity() : Entity(pixelData, _ActiveScene.get());
        }

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
                    if (ImGui::MenuItem("New", "Ctrl+N"))
                        NewScene();
                    if (ImGui::MenuItem("Open...", "Ctrl+O"))
                        OpenScene();
                    if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
                        SaveAsScene();
                    if (ImGui::MenuItem("Exit"))
                        Application::Get().Close();

                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }

            _SceneHierarchyPanel->OnImGuiRender();
            _PropertiesPanel->OnImGuiRender();
            _ContentBrowserPanel->OnImGuiRender();

            ImGui::Begin("Renderer2D Stats");

            auto stats = Renderer::R2D::GetStats();
            ImGui::Text("Renderer2D Stats: ");
            ImGui::Text("DrawCalls: %d", stats.DrawCalls);
            ImGui::Text("Rect Count: %d", stats.RectCount);
            ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
            ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
            ImGui::Text("FPS: %f", _FPS);
            ImGui::End();

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
            ImGui::Begin("Viewport");
            auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
            auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
            auto viewportOffset = ImGui::GetWindowPos();
            _ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
            _ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

            _bViewportFocus = ImGui::IsWindowFocused();
            _bViewportHovered = ImGui::IsWindowHovered();
            Application::Get().GetImGuiLayer()->SetBlockEvents(!_bViewportFocus && !_bViewportHovered);
            

            ImVec2 viewportSize = ImGui::GetContentRegionAvail();
            _ViewportSize = { viewportSize.x, viewportSize.y };

            uint32_t textureID = _FrameBuffer->GetColorAttachmentID();
            ImGui::Image((void *)textureID, ImVec2{ _ViewportSize.x, _ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

            if (ImGui::BeginDragDropTarget()) {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
                    const char* path = (const char*)payload->Data;
                    OpenScene(_AssetDirectory / path);
                }
                ImGui::EndDragDropTarget();
            }

            //ImGuizmo stuff
            Entity selectedEntity = _SceneHierarchyPanel->GetSelectedEntity();
            if (selectedEntity && _GizmoType != -1) {
                ImGuizmo::SetOrthographic(false);
                ImGuizmo::SetDrawlist();
                ImGuizmo::SetRect(_ViewportBounds[0].x, _ViewportBounds[0].y, _ViewportBounds[1].x - _ViewportBounds[0].x, _ViewportBounds[1].y - _ViewportBounds[0].y);

                //camera
                const glm::mat4 cameraProj = _EditorCamera.GetProjection();
                glm::mat4 cameraView = _EditorCamera.GetViewMatrix();
                //ImGuizmo::SetOrthographic((cameraComponent._ProjectionType == C_Camera::ProjectionType::Orthographic) ? true : false);

                //entity
                auto& transformComponent = selectedEntity.GetComponent<C_Transform>();
                glm::mat4 transform = transformComponent.GetTransform();

                //snapping
                bool snap = Input::IsKeyPressed(Key::LeftControl);
                float snapValue = (_GizmoType != ImGuizmo::OPERATION::ROTATE) ? 0.5f : 30.0f;

                float snapValues[3] = { snapValue, snapValue, snapValue };
                ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProj),
                    (ImGuizmo::OPERATION)_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
                    nullptr, snap ? snapValues : nullptr);

                if (ImGuizmo::IsUsing()) {

                    glm::vec3 scale;
                    glm::vec3 rotation;
                    glm::vec3 translation;
                    Math::DecomposeTransform(transform, translation, rotation, scale);

                    transformComponent.Translation = translation;
                    transformComponent.Rotation = rotation;
                    transformComponent.Scale = scale;
                }
            }

            ImGui::End();
            ImGui::PopStyleVar();

            ImGui::End();
    }

    void EditorLayer::OnEvent(Event& e)
    {
        _EditorCamera.OnEvent(e);

        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<KeyPressedEvent>(IMAGINE_BIND_EVENT(EditorLayer::OnKeyPressed));
        dispatcher.Dispatch<MouseButtonPressedEvent>(IMAGINE_BIND_EVENT(EditorLayer::OnMouseButtonPressed));
    }
    bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
    {
        bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
        bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);
        //shortcuts
        if (e.GetRepeatCount() > 0) {
            return false;
        }

        switch (e.GetKeyCode()) {
            case Key::N:
                if (control) {
                    NewScene();
                    break;
                }
            case Key::O:
                if (control) {
                    OpenScene();
                    break;
                }
            case Key::S:
                if (control && shift) {
                    SaveAsScene();
                    break;
                }                
        }
        switch (e.GetKeyCode()) {
            case Key::Q:
                _GizmoType = -1;
                break;
            case Key::W:
                _GizmoType = ImGuizmo::OPERATION::TRANSLATE;
                break;
            case Key::E:
                _GizmoType = ImGuizmo::OPERATION::ROTATE;
                break;
            case Key::R:
                _GizmoType = ImGuizmo::OPERATION::SCALE;
                break;
        }

        return false;
    }
    bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
    {
        //for mouse picking in the viewport
        if (e.GetMouseButton() == Mouse::ButtonLeft && _bViewportHovered && !ImGuizmo::IsOver() && !Input::IsKeyPressed(Key::LeftAlt)) {
            _SceneHierarchyPanel->SetSelectedEntity(_HoveredEntity);
        }
        return false;
    }
    void EditorLayer::NewScene()
    {
        _ActiveScene = CreateRefPtr<Scene>();
        _ActiveScene->OnViewportResize((size_t)_ViewportSize.x, (size_t)_ViewportSize.y);
        _SceneHierarchyPanel->SetContext(_ActiveScene);
    }
    void EditorLayer::OpenScene()
    {
        std::string filepath = FileDialogs::OpenFile("Imagine Scene (*.imsc)\0*.imsc\0");
        if (!filepath.empty()) {
            OpenScene(filepath);
        }
    }
    void EditorLayer::OpenScene(const std::filesystem::path& path)
    {
        _ActiveScene = CreateRefPtr<Scene>();
        SceneSerializer serializer(_ActiveScene);
        serializer.DeSerializeText(path.string());

        _ActiveScene->OnViewportResize((size_t)_ViewportSize.x, (size_t)_ViewportSize.y);
        _SceneHierarchyPanel->SetContext(_ActiveScene);
    }
    void EditorLayer::SaveAsScene()
    {
        std::string filepath = FileDialogs::SaveFile("Imagine Scene (*.imsc)\0*.imsc\0");
        std::string sceneName(std::filesystem::path(filepath).stem().string());
        _ActiveScene->SetName(sceneName);
        if (!filepath.empty()) {
            SceneSerializer serializer(_ActiveScene);
            serializer.SerializeText(filepath);
        }
    }
}