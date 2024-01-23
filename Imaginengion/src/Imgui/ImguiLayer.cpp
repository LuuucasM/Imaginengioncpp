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
}