#include "impch.h"
#include "Shader.h"

#include "RendererAPI.h"


namespace IM {

	Shader::Shader(const std::string& filepath) {

		_Filepath = filepath;
		std::filesystem::path path = filepath;
		_Name = path.stem().string();

		std::string source = ReadFile(_Filepath);
		PreProcess(source);
		CompileOrGetVulkanBinaries();
		CreateVertexBufferLayout();
		Reflect();
		CompileOrGetRenderPlatformBinaries();
		CreateProgram();
	}
	std::string Shader::ReadFile(const std::string& filepath){
		IMAGINE_PROFILE_FUNCTION();

		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in) {
			in.seekg(0, std::ios::end);

			size_t size = in.tellg();
			if (size != -1) {
				result.resize(in.tellg());
				in.seekg(0, std::ios::beg);
				in.read(&result[0], result.size());
				in.close();
			}
			else {
				IMAGINE_CORE_ERROR("Could not read from file '{}'", filepath);
			}
		}
		else {
			IMAGINE_CORE_ERROR("Could not open file '{}' in OpenGLShader", filepath);
		}
		return result;
	}
}