
-- Imaginengion Dependencies

VULKAN_SDK = os.getenv("VULKAN_SDK")

IncludeDir = {}
IncludeDir["stb_image"] = "%{wks.location}/Imaginengion/vendor/stb_image"
IncludeDir["yaml_cpp"] = "%{wks.location}/Imaginengion/vendor/yaml-cpp/include"
IncludeDir["GLFW"] = "%{wks.location}/Imaginengion/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/Imaginengion/vendor/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/Imaginengion/vendor/imgui"
IncludeDir["ImGuizmo"] = "%{wks.location}/Imaginengion/vendor/ImGuizmo"
IncludeDir["glm"] = "%{wks.location}/Imaginengion/vendor/glm"
IncludeDir["shaderc"] = "%{wks.location}/Imaginengion/vendor/shaderc/include"
IncludeDir["SPIRV_Cross"] = "%{wks.location}/Imaginengion/vendor/SPIRV-Cross"
IncludeDir["VulkanSDK"] = "%{wks.location}/Imaginengion/vendor/VulkanSDK/Include"
IncludeDir["Box2D"] = "%{wks.location}/Imaginengion/vendor/Box2D/include"

LibraryDir = {}

LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"
LibraryDir["VulkanSDK_Debug"] = "%{wks.location}/Imaginengion/vendor/VulkanSDK/Lib"

Library = {}
Library["Vulkan"] = "%{LibraryDir.VulkanSDK_Debug}/vulkan-1.lib"
Library["VulkanUtils"] = "%{LibraryDir.VulkanSDK_Debug}/VkLayer_utils.lib"

Library["ShaderC_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/shaderc_sharedd.lib"
--Library["SPIRV_Cross_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/spirv-cross-cored.lib"
--Library["SPIRV_Cross_GLSL_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/spirv-cross-glsld.lib"
--Library["SPIRV_Tools_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/SPIRV-Toolsd.lib"

Library["ShaderC_Release"] = "%{LibraryDir.VulkanSDK_Debug}/shaderc_shared.lib"
--Library["SPIRV_Cross_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-core.lib"
--Library["SPIRV_Cross_GLSL_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsl.lib"