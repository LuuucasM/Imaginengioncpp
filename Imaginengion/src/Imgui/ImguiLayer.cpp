#include "impch.h"
#include "ImguiLayer.h"


//#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"

#include "Core/Application.h"

#include <GLFW/glfw3.h>
#include <ImGuizmo.h>

namespace IM {

	ImguiLayer::ImguiLayer()
		: Layer("ImguiLayer") {

	}

	void ImguiLayer::OnAttach() {

        IMAGINE_PROFILE_FUNCTION();

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
        //io.ConfigViewportsNoAutoMerge = true;
        //io.ConfigViewportsNoTaskBarIcon = true;

        io.Fonts->AddFontFromFileTTF("assets/fonts/noto sans/static/NotoSans-Bold.ttf", 16.0);
        io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/noto sans/static/NotoSans-Regular.ttf", 16.0);

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsLight();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        SetDarkThemeColors();

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow()), true);
        ImGui_ImplOpenGL3_Init("#version 460");
	}

	void ImguiLayer::OnDetach() {

        IMAGINE_PROFILE_FUNCTION();

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
	}

    void ImguiLayer::OnEvent(Event& e)
    {
        if (_bBlockEvents) {
            ImGuiIO& io = ImGui::GetIO();
            e._bHandled |= e.IsInCategory(EC_Mouse) & io.WantCaptureMouse;
            e._bHandled |= e.IsInCategory(EC_Keyboard) & io.WantCaptureKeyboard;
        }
    }

    void ImguiLayer::Begin() {

        IMAGINE_PROFILE_FUNCTION();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGuizmo::BeginFrame();


    }
    void ImguiLayer::End() {

        IMAGINE_PROFILE_FUNCTION();

        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::Get();
        io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }
    void ImguiLayer::SetDarkThemeColors()
    {
        auto& colors = ImGui::GetStyle().Colors;

        colors[ImGuiCol_WindowBg] = ImVec4{ 0.082f, 0.082f, 0.082f, 1.0f };


        //Headers
        colors[ImGuiCol_Header] = ImVec4{ 0.176f, 0.190f, 0.229f, 1.0f };
        colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.293f, 0.317f, 0.382f, 1.0f };
        colors[ImGuiCol_HeaderActive] = ImVec4{ 0.235f, 0.254f, 0.306f, 1.0f };
        
        //Buttons
        colors[ImGuiCol_Button] = ImVec4{ 0.250f, 0.266f, 0.358f, 1.0f };
        colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.375f, 0.399f, 0.537f, 1.0f };
        colors[ImGuiCol_ButtonActive] = ImVec4{ 0.312f, 0.332f, 0.447f, 1.0f };
        //Frame BG
        colors[ImGuiCol_FrameBg] = ImVec4{ 0.317f, 0.290f, 0.290f, 1.0f };
        colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.475f, 0.435f, 0.435f, 1.0f };
        colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.396f, 0.362f, 0.362f, 1.0f };
        
        //Tabs
        colors[ImGuiCol_Tab] = ImVec4{ 0.8f, 0.3f, 0.3f, 1.0f };
        colors[ImGuiCol_TabHovered] = ImVec4{ 0.45f, 0.252f, 0.592f, 1.0f };
        //colors[ImGuiCol_TabActive] = ImVec4{ 0.317f, 0.484f, 0.552f, 1.0f };
        colors[ImGuiCol_TabActive] = ImVec4{ 0.375f, 0.21f, 0.493f, 1.0f };
        colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.8f, 0.3f, 0.3f, 1.0f };
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.300f, 0.168f, 0.395f, 1.0f };
        
        //titles
        colors[ImGuiCol_TitleBg] = ImVec4{ 0.094f, 0.052f, 0.123f, 1.0f };
        colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.188f, 0.104f, 0.246f, 1.0f };
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.8f, 0.3f, 0.3f, 1.0f };

        colors[ImGuiCol_ResizeGrip] = ImVec4{ 0.8f, 0.3f, 0.3f, 1.0f };           // Resize grip in lower-right and lower-left corners of windows.
        colors[ImGuiCol_ResizeGripHovered] = ImVec4{ 0.303f, 0.333f, 0.340f, 1.0f };
        colors[ImGuiCol_ResizeGripActive] = ImVec4{ 0.404f, 0.444f, 0.454f, 1.0f };
    }
}