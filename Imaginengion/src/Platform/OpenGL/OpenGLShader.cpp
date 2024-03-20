#include "impch.h"
#include "OpenGLShader.h"

#include <sstream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_glsl.hpp>

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
	}


	OpenGLShader::OpenGLShader(const std::string& filepath) {

		IMAGINE_PROFILE_FUNCTION();

		std::string source = ReadFile(filepath);
		auto shaderSources = PreProcess(source);
		if (Compile(shaderSources)) {
			//CreateLayout
			CreateLayout(shaderSources[GL_VERTEX_SHADER]);
			//DiscoverUniforms
			DiscoverUniforms();
		}

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

	bool OpenGLShader::Compile(std::unordered_map<GLenum, std::string> shaderSources) {

		IMAGINE_PROFILE_FUNCTION();

		GLuint program = glCreateProgram();
		IMAGINE_CORE_ASSERT(shaderSources.size() <= 4, "Only support up to 4 shaders in OpenGLShader Compile")
		std::array<GLenum, 4> glShaderIDs;
		int glShaderIndex = 0;

		for (const auto& [type, source] : shaderSources) {
			GLuint shader = glCreateShader(type);
			// Send the vertex shader source code to GL
			// Note that std::string's .c_str is NULL character terminated.
			const char* source_cstr = source.c_str();
			glShaderSource(shader, 1, &source_cstr, 0);

			// Compile the shader
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(shader);

				// Use the infoLog as you see fit.
				IMAGINE_CORE_ERROR("Shader compilation failure! {}", infoLog.data());

				// In this simple program, we'll just leave
				return false;
			}
			glAttachShader(program, shader);
			glShaderIDs[glShaderIndex++] = shader;
		}

		_ProgramID = program;

		// Link our program
		glLinkProgram(_ProgramID);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(_ProgramID, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(_ProgramID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(_ProgramID, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(_ProgramID);
			// Don't leak shaders either.

			for (int i = 0; i < shaderSources.size(); ++i) {
				glDeleteShader(glShaderIDs[i]);
			}
			// Use the infoLog as you see fit.
			IMAGINE_CORE_ERROR("{}", infoLog.data());
			IMAGINE_CORE_ASSERT(false, "Shader Program linking failure!");
			// In this simple program, we'll just leave
			return false;
		}

		// Always detach shaders after a successful link.
		for (int i = 0; i < shaderSources.size(); ++i) {
			glDetachShader(_ProgramID, glShaderIDs[i]);
			glDeleteShader(glShaderIDs[i]);
		}
		return true;
	}

	void OpenGLShader::CompileOrGetVulkanBinaries(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
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
		if (!std::filesystem::exists("assets/cache/shader/opengl")) {
			std::filesystem::create_directories("assets/cache/shader/opengl");
		}
		std::filesystem::path cacheDirectory = "assets/cache/shader/opengl";

		shaderData.clear();
		_OpenGLSourceCode.clear();
		for (auto&& [stage, spirv] : _VulkanSPIRV) {
			std::filesystem::path shaderFilePath = _Filepath;
			std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.filename().string() + );
				//NOTE I AM HERE RIGHT NOW AT COMPILER OR GET OPENGL BINARIES.
				//I AM GOING TO ADD THE STATIC FUNCTIONS GETCACHEDIRECTORY AND CREATECACHEDIRECTORYIFNEEDED
				//THEN I WILL DO A SMALL CHANGE ABOVE FOR THE FILE PATH STUFF AND THEN CONTINUE CODING OUT THE REST OF THIS FILE !
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

	void OpenGLShader::Reflect(GLenum stage, const std::vector<uint32_t>& shaderData)
	{
		spirv_cross::Compiler compiler(shaderData);
		spirv_cross::ShaderResources resources = compiler.get_shader_resources();

		IMAGINE_CORE_TRACE("OpenGLShader::Reflect - {} {}", GLShaderStageToString(stage), _Filepath);

		IMAGINE_CORE_TRACE("Stage Inputs:");
		std::sort(resources.stage_inputs.begin(), resources.stage_inputs.end(),
			[compiler](const spirv_cross::Resource& a, const spirv_cross::Resource& b) {
				return compiler.get_decoration(a.id, spv::DecorationLocation) < compiler.get_decoration(b.id, spv::DecorationLocation);
			});
		for (const auto& resource : resources.stage_inputs) {
			const auto& type = compiler.get_type(resource.base_type_id);
			const std::string name = compiler.get_name(resource.id);
			IMAGINE_CORE_TRACE("\t{}", name);
			_BufferElements.emplace_back(SpirTypeToType(type), name);
		}
		CalculateOffsets();
		CalculateStride();

		IMAGINE_CORE_TRACE("UniformBuffers:");
		for (const auto& resource : resources.uniform_buffers) {
			IMAGINE_CORE_TRACE("\t{}", resource.name);
		}
	}

	void OpenGLShader::CreateLayout(const std::string& vertexShader)
	{
		std::istringstream shaderStream(vertexShader);
		std::string line;

		while (std::getline(shaderStream, line)) {
			if (line.find("layout") != std::string::npos && line.find("in") != std::string::npos) {
				// Extracting variable name and type from the line
				size_t startPos = line.find("in") + 3;  // position after "in"
				size_t endPos = line.find(";", startPos);
				std::string variableInfo = line.substr(startPos, endPos - startPos);

				// Parsing variable information
				std::istringstream variableStream(variableInfo);
				std::string type, name;
				variableStream >> type >> name;

				// Removing any leading/trailing whitespace from the name
				size_t nameStart = name.find_first_not_of(" \t");
				size_t nameEnd = name.find_last_not_of(" \t");
				name = name.substr(nameStart, nameEnd - nameStart + 1);

				_BufferElements.emplace_back(StrToDataType[type], name);
			}
		}
		CalculateOffsets();
		CalculateStride();
	}

	void OpenGLShader::DiscoverUniforms()
	{
		GLint i;
		GLint count;
		GLint size;
		GLenum type;

		const GLsizei bufSize = 32;
		GLchar uniname[bufSize];
		GLsizei length;

		glGetProgramiv(_ProgramID, GL_ACTIVE_UNIFORMS, &count);

		for (i = 0; i < count; i++) {
			glGetActiveUniform(_ProgramID, (GLuint)i, bufSize, &length, &size, &type, uniname);
			std::string name(uniname);
			if (size_t found = name.find("["); found != std::string::npos) {
				name.erase(found, name.size()-found);
			}
			_Uniforms[name] = glGetUniformLocation(_ProgramID, name.c_str());
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