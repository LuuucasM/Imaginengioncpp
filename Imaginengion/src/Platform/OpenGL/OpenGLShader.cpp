#include "impch.h"
#include "OpenGLShader.h"

#include <fstream>
#include <sstream>
#include <regex>
#include <glad/glad.h>

namespace IM {

	static GLenum ShaderTypeFromStr(const std::string& type) {
		if (type.find("vertex") != std::string::npos) {
			return GL_VERTEX_SHADER;
		}
		if (type.find("fragment") != std::string::npos || type.find("pixel") != std::string::npos) {
			return GL_FRAGMENT_SHADER;
		}

		IMAGINE_CORE_ASSERT(false, "Unknown shader type in OpenGLShader!");
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) {

		IMAGINE_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> shaderSources;
		shaderSources[GL_VERTEX_SHADER] = vertexSrc;
		shaderSources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(shaderSources);

		_Name = name;
	}

	OpenGLShader::OpenGLShader(const std::string& filepath) {

		IMAGINE_PROFILE_FUNCTION();

		std::string source = ReadFile(filepath);
		auto shaderSources = PreProcess(source);
		Compile(shaderSources);

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

	void OpenGLShader::Compile(std::unordered_map<GLenum, std::string> shaderSources) {

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
				return;
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
			IMAGINE_CORE_ASSERT(false, "Shader Program linking failure!\n {0}", infoLog.data());
			// In this simple program, we'll just leave
			return;
		}

		// Always detach shaders after a successful link.
		for (int i = 0; i < shaderSources.size(); ++i) {
			glDetachShader(_ProgramID, glShaderIDs[i]);
			glDeleteShader(glShaderIDs[i]);
		}
	}

	void OpenGLShader::SetValue(const std::string& name, bool value) {
		GLint location = glGetUniformLocation(_ProgramID, name.c_str());
		IMAGINE_CORE_ASSERT(location != -1, "Cannot find union with the given name in OpenGLShader::SetBool");
		glUniform1i(location, (int)value);
	}
	void OpenGLShader::SetValue(const std::string& name, int value) {
		/*
		GLint i;
		GLint count;
		GLint size;
		GLenum type;

		const GLsizei bufSize = 16;
		GLchar uniname[bufSize];
		GLsizei length;

		glGetProgramiv(_ProgramID, GL_ACTIVE_UNIFORMS, &count);
		std::cout << "Active Uniforms: " << count << std::endl;

		for (i = 0; i < count; i++) {
			glGetActiveUniform(_ProgramID, (GLuint)i, bufSize, &length, &size, &type, uniname);
			std::cout << "Uniform #" << i << "Type: " << type << "Name: " << uniname << std::endl;
		}
		*/
		GLint location = glGetUniformLocation(_ProgramID, name.c_str());
		IMAGINE_CORE_ASSERT(location != -1, "Cannot find union with the given name in OpenGLShader::SetInt");
		glUniform1i(location, value);
	}
	void OpenGLShader::SetValue(const std::string& name, int* values, uint32_t count)
	{
		GLint location = glGetUniformLocation(_ProgramID, name.c_str());
		IMAGINE_CORE_ASSERT(location != -1, "Cannot find union with the given name in OpenGLShader::SetIntArray");
		glUniform1iv(location, count, values);
	}
	void OpenGLShader::SetValue(const std::string& name, float value) {
		GLint location = glGetUniformLocation(_ProgramID, name.c_str());
		IMAGINE_CORE_ASSERT(location != -1, "Cannot find union with the given name in OpenGLShader::SetFloat");
		glUniform1f(location, value);
	}
	void OpenGLShader::SetValue(const std::string& name, glm::vec2 value) {
		GLint location = glGetUniformLocation(_ProgramID, name.c_str());
		IMAGINE_CORE_ASSERT(location != -1, "Cannot find union with the given name in OpenGLShader::Setglm::vec2");
		glUniform2f(location, value.x, value.y);
	}
	void OpenGLShader::SetValue(const std::string& name, glm::vec3 value) {
		GLint location = glGetUniformLocation(_ProgramID, name.c_str());
		IMAGINE_CORE_ASSERT(location != -1, "Cannot find union with the given name in OpenGLShader::Setglm::vec3");
		glUniform3f(location, value.x, value.y, value.z);
	}
	void OpenGLShader::SetValue(const std::string& name, glm::vec4 value) {
		GLint location = glGetUniformLocation(_ProgramID, name.c_str());
		IMAGINE_CORE_ASSERT(location != -1, "Cannot find union with the given name in OpenGLShader::Setglm::vec4");
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}
	void OpenGLShader::SetValue(const std::string& name, glm::mat3 value) {
		GLint location = glGetUniformLocation(_ProgramID, name.c_str());
		IMAGINE_CORE_ASSERT(location != -1, "Cannot find union with the given name in OpenGLShader::Setglm::mat3");
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}
	void OpenGLShader::SetValue(const std::string& name, glm::mat4 value) {
		GLint location = glGetUniformLocation(_ProgramID, name.c_str());
		IMAGINE_CORE_ASSERT(location != -1, "Cannot find union with the given name in OpenGLShader::Setglm::mat4");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}
}