#include "impch.h"
#include "ImguiLayer.h"


//#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"

#include "Core/Application.h"

#include <GLFW/glfw3.h>

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

        colors[ImGuiCol_WindowBg] = ImVec4{ 0.032f, 0.047f, 0.048f, 1.0f };


        //Headers
        colors[ImGuiCol_Header] = ImVec4{ 0.204f, 0.194f, 0.312f, 1.0f };
        colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.306f, 0.291f, 0.468f, 1.0f };
        colors[ImGuiCol_HeaderActive] = ImVec4{ 0.255f, 0.242f, 0.39f, 1.0f };
        
        //Buttons
        colors[ImGuiCol_Button] = ImVec4{ 0.203f, 0.283f, 0.376f, 1.0f };
        colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.304f, 0.424f, 0.564f, 1.0f };
        colors[ImGuiCol_ButtonActive] = ImVec4{ 0.253f, 0.353f, 0.470f, 1.0f };
        //Frame BG
        colors[ImGuiCol_FrameBg] = ImVec4{ 0.127f, 0.177f, 0.235f, 1.0f };
        colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.190f, 0.265f, 0.352f, 1.0f };
        colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.158f, 0.221f, 0.293f, 1.0f };
        
        //Tabs
        colors[ImGuiCol_Tab] = ImVec4{ 0.8f, 0.3f, 0.3f, 1.0f };
        colors[ImGuiCol_TabHovered] = ImVec4{ 0.8f, 0.3f, 0.3f, 1.0f };
        colors[ImGuiCol_TabActive] = ImVec4{ 0.317f, 0.484f, 0.552f, 1.0f };
        colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.8f, 0.3f, 0.3f, 1.0f };
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.185f, 0.282f, 0.322f, 1.0f };
        
        //titles
        colors[ImGuiCol_TitleBg] = ImVec4{ 0.052f, 0.080f, 0.092f, 1.0f };
        colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.078f, 0.120f, 0.138f, 1.0f };
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.8f, 0.3f, 0.3f, 1.0f };
    }
}