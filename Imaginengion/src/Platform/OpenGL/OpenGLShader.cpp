#include "impch.h"
#include "OpenGLShader.h"

#include <sstream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <shaderc/shaderc.hpp>
#include <spirv_cross.hpp>
#include <spirv_glsl.hpp>

namespace IM {

	namespace {
		GLenum ShaderTypeFromStr(const std::string& type) {
			if (type.find("vertex") != std::string::npos) {
				return GL_VERTEX_SHADER;
			}
			if (type.find("fragment") != std::string::npos || type.find("pixel") != std::string::npos) {
				return GL_FRAGMENT_SHADER;
			}

			IMAGINE_CORE_ASSERT(false, "Unknown shader type in OpenGLShader!");
			return 0;
		}

		const char* GLShaderStageToString(GLenum stage) {
			switch (stage) {
				case GL_VERTEX_SHADER: return "GL_VERTEX_SHADER";
				case GL_FRAGMENT_SHADER: return "GL_FRAGMENT_SHADER";
			}
			IMAGINE_CORE_ASSERT(0, "stage unknown in GLShaderStageToString !");
			return nullptr;
		}

		const char* CreateCacheDirectoryIfNeeded() {
			if (!std::filesystem::exists("assets/cache/shader/opengl")) {
				std::filesystem::create_directories("assets/cache/shader/opengl");
			}
			return "assets/cache/shader/opengl";
		}

		const char* GetCacheDirectory() {
			return CreateCacheDirectoryIfNeeded();
		}

		const char* GLShaderStageCachedOpenGLFileExtension(uint32_t stage) {
			switch (stage) {
				case GL_VERTEX_SHADER: return ".cached_opengl.vert";
				case GL_FRAGMENT_SHADER: return ".cached_opengl.frag";
			}
			IMAGINE_CORE_ASSERT(0, "unknown shader in GLShaderStageCachedOpenGLFileExtension!");
			return "";
		}

		const char* GLShaderStageCachedVulkanFileExtension(uint32_t stage) {
			switch (stage) {
			case GL_VERTEX_SHADER: return ".cached_vulkan.vert";
			case GL_FRAGMENT_SHADER: return ".cached_vulkan.frag";
			}
			IMAGINE_CORE_ASSERT(0, "unknown shader in GLShaderStageCachedVulkanFileExtension!");
			return "";
		}

		shaderc_shader_kind GLShaderStageToShaderC(GLenum stage) {
			switch (stage) {
				case GL_VERTEX_SHADER: return shaderc_glsl_vertex_shader;
				case GL_FRAGMENT_SHADER: return shaderc_glsl_fragment_shader;
			}
			IMAGINE_CORE_ASSERT(0, "unkown shader in GLShaderStageToShaderC");
			return (shaderc_shader_kind)0;
		}

		static ShaderDataType SpirTypeToType(spirv_cross::SPIRType type) {
			if (type.vecsize == 1 && type.columns == 1) {
				if (type.basetype == spirv_cross::SPIRType::Float) {
					return ShaderDataType::Float;
				}
				else if (type.basetype == spirv_cross::SPIRType::Int) {
					return ShaderDataType::Int;
				}
				else if (type.basetype == spirv_cross::SPIRType::UInt) {
					return ShaderDataType::UInt;
				}
				else if (type.basetype == spirv_cross::SPIRType::Boolean) {
					return ShaderDataType::Int;
				}
			}
			if (type.vecsize == 2 && type.columns == 1) {
				if (type.basetype == spirv_cross::SPIRType::Float) {
					return ShaderDataType::Float2;
				}
				if (type.basetype == spirv_cross::SPIRType::Int) {
					return ShaderDataType::Int2;
				}
			}
			if (type.vecsize == 3 && type.columns == 1) {
				if (type.basetype == spirv_cross::SPIRType::Float) {
					return ShaderDataType::Float3;
				}
				if (type.basetype == spirv_cross::SPIRType::Int) {
					return ShaderDataType::Int3;
				}
			}
			if (type.vecsize == 4 && type.columns == 1) {
				if (type.basetype == spirv_cross::SPIRType::Float) {
					return ShaderDataType::Float4;
				}
				if (type.basetype == spirv_cross::SPIRType::Int) {
					return ShaderDataType::Int4;
				}
			}
			if (type.vecsize == 3 && type.columns == 3) {
				return ShaderDataType::Mat3;
			}
			if (type.vecsize == 4 && type.columns == 4) {
				return ShaderDataType::Mat4;
			}
			IMAGINE_CORE_ASSERT(0, "Unknown spirv_cross::SPIRType in SpirTypeToType");
			return ShaderDataType::None;
		}
	}


	OpenGLShader::OpenGLShader(const std::string& filepath){

		IMAGINE_PROFILE_FUNCTION();

		_Filepath = filepath;

		std::string source = ReadFile(filepath);
		auto shaderSources = PreProcess(source);
		CompileOrGetVulkanBinaries(shaderSources);
		CompileOrGetOpenGLBinaries();
		CreateProgram();

		//extract name from filepath
		std::filesystem::path path = filepath;
		_Name = path.stem().string();
	}
	OpenGLShader::~OpenGLShader() {

		IMAGINE_PROFILE_FUNCTION();

		glDeleteProgram(_ProgramID);
	}

	void OpenGLShader::Bind() const {

		IMAGINE_PROFILE_FUNCTION();

		glUseProgram(_ProgramID);
	}
	void OpenGLShader::Unbind() const {
		glUseProgram(0);
	}

	std::string OpenGLShader::ReadFile(const std::string& filepath) {

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

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(std::string& source) {

		IMAGINE_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> shaderSources;

		std::istringstream stream(source);
		std::string line;
		std::string currentShaderType;
		std::string currentShaderSource;

		while (std::getline(stream, line)) {
			if (line.find("#type") != std::string::npos) {
				// Save the previous shader (if any)
				if (!currentShaderType.empty() && !currentShaderSource.empty()) {
					shaderSources[ShaderTypeFromStr(currentShaderType)] = currentShaderSource;
				}

				// Extract shader type for the new shader
				currentShaderType = line.substr(line.find("#type") + 6);
				currentShaderSource = "";
			}
			else {
				// Collect shader source
				currentShaderSource += line + "\n";
			}
		}

		// Save the last shader (if any)
		if (!currentShaderType.empty() && !currentShaderSource.empty()) {
			shaderSources[ShaderTypeFromStr(currentShaderType)] = currentShaderSource;
		}
		return shaderSources;
	}

	void OpenGLShader::CompileOrGetVulkanBinaries(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		GLuint program = glCreateProgram();

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
		const bool optimize = false; //TODO: do something about this later
		if (optimize) {
			options.SetOptimizationLevel(shaderc_optimization_level_performance);
		}

		std::filesystem::path cacheDirectory = GetCacheDirectory();

		auto shaderData = _VulkanSPIRV;
		shaderData.clear();
		for (auto&& [stage, source] : shaderSources) {
			std::filesystem::path shaderFilePath = _Filepath;
			std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.filename().string() + GLShaderStageCachedVulkanFileExtension(stage));

			std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
			if (in.is_open()) {
				in.seekg(0, std::ios::end);
				auto size = in.tellg();
				in.seekg(0, std::ios::beg);

				auto& data = shaderData[stage];
				data.resize(size / sizeof(uint32_t));
				in.read((char*)data.data(), size);
			}
			else {
				shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, GLShaderStageToShaderC(stage), _Filepath.c_str(), options);
				if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
					IMAGINE_CORE_ERROR(module.GetErrorMessage());
					IMAGINE_CORE_ASSERT(0);
				}

				shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

				std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
				if (out.is_open()) {
					auto& data = shaderData[stage];
					out.write((char*)data.data(), data.size() * sizeof(uint32_t));
					out.flush();
					out.close();
				}
			}
		}
		CreateBufferLayout(shaderData[GL_VERTEX_SHADER]);
		for (auto&& [stage, data] : shaderData) {
			Reflect(stage, data);
		}
	}

	void OpenGLShader::CompileOrGetOpenGLBinaries()
	{
		auto& shaderData = _OpenGLSPIRV;

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
		const bool optimize = false; //TODO: do something about this later
		if (optimize) {
			options.SetOptimizationLevel(shaderc_optimization_level_performance);
		}

		std::filesystem::path cacheDirectory = GetCacheDirectory();

		shaderData.clear();
		_OpenGLSourceCode.clear();
		for (auto&& [stage, spirv] : _VulkanSPIRV) {
			std::filesystem::path shaderFilePath = _Filepath;
			std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.filename().string() + GLShaderStageCachedOpenGLFileExtension(stage));
			
			std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
			if (in.is_open()) {
				in.seekg(0, std::ios::end);
				auto size = in.tellg();
				in.seekg(0, std::ios::beg);

				auto& data = shaderData[stage];
				data.resize(size / sizeof(uint32_t));
				in.read((char*)data.data(), size);
			}
			else {
				spirv_cross::CompilerGLSL glslCompiler(spirv);
				_OpenGLSourceCode[stage] = glslCompiler.compile();
				auto& source = _OpenGLSourceCode[stage];

				shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, GLShaderStageToShaderC(stage), _Filepath.c_str(), options);
				if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
					IMAGINE_CORE_ERROR(module.GetErrorMessage());
					IMAGINE_CORE_ASSERT(0);
				}

				shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());
				std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
				if (out.is_open()) {
					auto& data = shaderData[stage];
					out.write((char*)data.data(), data.size() * sizeof(uint32_t));
					out.flush();
					out.close();
				}
			}
		}

	}

	void OpenGLShader::CreateProgram()
	{
		GLuint program = glCreateProgram();
		std::vector<GLuint> shaderIDs;
		for (auto&& [stage, spirv] : _OpenGLSPIRV) {
			GLuint shaderID = shaderIDs.emplace_back(glCreateShader(stage));
			glShaderBinary(1, &shaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, spirv.data(), spirv.size() * sizeof(uint32_t));
			glSpecializeShader(shaderID, "main", 0, nullptr, nullptr);
			glAttachShader(program, shaderID);
		}
		glLinkProgram(program);
		GLint isLinked;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE) {
			GLint maxLength;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data());
			IMAGINE_CORE_ERROR("Shader linking failed ({}):\n{}", _Filepath, infoLog.data());
			glDeleteProgram(program);
			for (auto id : shaderIDs) {
				glDeleteShader(id);
			}
		}
		for (auto id : shaderIDs) {
			glDetachShader(program, id);
			glDeleteShader(id);
		}
		_ProgramID = program;
	}

	void OpenGLShader::CreateBufferLayout(std::vector<uint32_t>& shaderData) {
		spirv_cross::Compiler compiler(shaderData);
		spirv_cross::ShaderResources resources = compiler.get_shader_resources();

		std::sort(resources.stage_inputs.begin(), resources.stage_inputs.end(),
			[&compiler](const spirv_cross::Resource& a, const spirv_cross::Resource& b) {
				return compiler.get_decoration(a.id, spv::DecorationLocation) < compiler.get_decoration(b.id, spv::DecorationLocation);
			});
		for (const auto& resource : resources.stage_inputs) {
			const auto& type = compiler.get_type(resource.base_type_id);
			const std::string name = compiler.get_name(resource.id);
			_BufferElements.emplace_back(SpirTypeToType(type), name);
		}

		CalculateOffsets();
		CalculateStride();
	}

	void OpenGLShader::Reflect(GLenum stage, const std::vector<uint32_t>& shaderData)
	{
		spirv_cross::Compiler compiler(shaderData);
		spirv_cross::ShaderResources resources = compiler.get_shader_resources();

		IMAGINE_CORE_TRACE("OpenGLShader::Reflect - {} {}", GLShaderStageToString(stage), _Filepath);

		IMAGINE_CORE_TRACE("Stage Inputs:");
		for (const auto& resource : resources.stage_inputs) {
			const auto& type = compiler.get_type(resource.base_type_id);
			const std::string name = compiler.get_name(resource.id);
		}
		IMAGINE_CORE_TRACE("UniformBuffers:");
		for (const auto& resource : resources.uniform_buffers) {
			IMAGINE_CORE_TRACE("\t{}", resource.name);
		}
	}

	void OpenGLShader::SetValue(const std::string& name, bool value) {
		IMAGINE_CORE_ASSERT(_Uniforms.contains(name), "Cannot find union with the given name in OpenGLShader::Setbool");
		glUniform1i(_Uniforms[name], (int)value);
	}
	void OpenGLShader::SetValue(const std::string& name, int value) {
		IMAGINE_CORE_ASSERT(_Uniforms.contains(name), "Cannot find union with the given name in OpenGLShader::SetInt");
		glUniform1i(_Uniforms[name], value);
	}
	void OpenGLShader::SetValue(const std::string& name, int* values, uint32_t count)
	{
		IMAGINE_CORE_ASSERT(_Uniforms.contains(name), "Cannot find union with the given name in OpenGLShader::SetIntArray");
		glUniform1iv(_Uniforms[name], count, values);
	}
	void OpenGLShader::SetValue(const std::string& name, float value) {
		IMAGINE_CORE_ASSERT(_Uniforms.contains(name), "Cannot find union with the given name in OpenGLShader::SetFloat");
		glUniform1f(_Uniforms[name], value);
	}
	void OpenGLShader::SetValue(const std::string& name, glm::vec2 value) {
		IMAGINE_CORE_ASSERT(_Uniforms.contains(name), "Cannot find union with the given name in OpenGLShader::SetVec2");
		glUniform2f(_Uniforms[name], value.x, value.y);
	}
	void OpenGLShader::SetValue(const std::string& name, glm::vec3 value) {
		IMAGINE_CORE_ASSERT(_Uniforms.contains(name), "Cannot find union with the given name in OpenGLShader::SetVec3");
		glUniform3f(_Uniforms[name], value.x, value.y, value.z);
	}
	void OpenGLShader::SetValue(const std::string& name, glm::vec4 value) {
		IMAGINE_CORE_ASSERT(_Uniforms.contains(name), "Cannot find union with the given name in OpenGLShader::SetVec4");
		glUniform4f(_Uniforms[name], value.x, value.y, value.z, value.w);
	}
	void OpenGLShader::SetValue(const std::string& name, glm::mat3 value) {
		IMAGINE_CORE_ASSERT(_Uniforms.contains(name), "Cannot find union with the given name in OpenGLShader::SetMat3");
		glUniformMatrix3fv(_Uniforms[name], 1, GL_FALSE, glm::value_ptr(value));
	}
	void OpenGLShader::SetValue(const std::string& name, glm::mat4 value) {
		IMAGINE_CORE_ASSERT(_Uniforms.contains(name), "Cannot find union with the given name in OpenGLShader::SetMat4");
		glUniformMatrix4fv(_Uniforms[name], 1, GL_FALSE, glm::value_ptr(value));
	}
}