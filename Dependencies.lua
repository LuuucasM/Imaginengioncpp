
-- Hazel Dependencies

VULKAN_SDK = os.getenv("VULKAN_SDK")

IncludeDir = {}
IncludeDir["stb_image"] = "%{wks.location}/Imaginengion/Vendor/stb_image"
IncludeDir["yaml_cpp"] = "%{wks.location}/Imaginengion/Vendor/yaml-cpp/include"
IncludeDir["GLFW"] = "%{wks.location}/Imaginengion/Vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/Imaginengion/Vendor/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/Imaginengion/Vendor/ImGui"
IncludeDir["ImGuizmo"] = "%{wks.location}/Imaginengion/Vendor/ImGuizmo"
IncludeDir["glm"] = "%{wks.location}/Imaginengion/Vendor/glm"
IncludeDir["entt"] = "%{wks.location}/Imaginengion/Vendor/entt/include"
IncludeDir["shaderc"] = "%{wks.location}/Imaginengion/Vendor/shaderc/include"
IncludeDir["SPIRV_Cross"] = "%{wks.location}/Imaginengion/Vendor/SPIRV-Cross"
IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"

LibraryDir = {}

LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"
LibraryDir["VulkanSDK_Debug"] = "%{wks.location}/Hazel/vendor/VulkanSDK/Lib"

Library = {}
Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
Library["VulkanUtils"] = "%{LibraryDir.VulkanSDK}/VkLayer_utils.lib"

Library["ShaderC_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/shaderc_sharedd.lib"
Library["SPIRV_Cross_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/spirv-cross-cored.lib"
Library["SPIRV_Cross_GLSL_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/spirv-cross-glsld.lib"
Library["SPIRV_Tools_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/SPIRV-Toolsd.lib"

Library["ShaderC_Release"] = "%{LibraryDir.VulkanSDK}/shaderc_shared.lib"
Library["SPIRV_Cross_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-core.lib"
Library["SPIRV_Cross_GLSL_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsl.lib"