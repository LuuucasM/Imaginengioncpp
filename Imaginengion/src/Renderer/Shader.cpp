#include "impch.h"
#include "Shader.h"

#include <glad/glad.h>

namespace IM {
	Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc) {

		// Create an empty vertex shader handle
		uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		const char *source = vertexSrc.c_str();
		glShaderSource(vertexShader, 1, &source, 0);

		// Compile the vertex shader
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(vertexShader);

			// Use the infoLog as you see fit.
			//IMAGINE_CORE_ASSERT(false, "Vertex Shader compilation failure!\n {0}", infoLog.data());

			// In this simple program, we'll just leave
			return;
		}

		// Create an empty fragment shader handle
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Send the fragment shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		source = (const GLchar*)fragmentSrc.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		// Compile the fragment shader
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(fragmentShader);
			// Either of them. Don't leak shaders.
			glDeleteShader(vertexShader);

			// Use the infoLog as you see fit.
			//IMAGINE_CORE_ASSERT(false, "Fragment Shader compilation failure!\n {0}", infoLog.data());

			// In this simple program, we'll just leave
			return;
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		_ProgramID = glCreateProgram();

		// Attach our shaders to our program
		glAttachShader(_ProgramID, vertexShader);
		glAttachShader(_ProgramID, fragmentShader);

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
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			// Use the infoLog as you see fit.
			//IMAGINE_CORE_ASSERT(false, "Shader Linking failure!\n {0}", infoLog.data());
			// In this simple program, we'll just leave
			return;
		}

		// Always detach shaders after a successful link.
		glDetachShader(_ProgramID, vertexShader);
		glDetachShader(_ProgramID, fragmentShader);
	}
	Shader::~Shader() {
		glDeleteProgram(_ProgramID);
	}

	void Shader::Bind() const {
		glUseProgram(_ProgramID);
	}
	void Shader::Unbind() const {
		glUseProgram(0);
	}

	void Shader::SetUniform(const std::string& name, bool value) {
		GLint location = glGetUniformLocation(_ProgramID, name.c_str());
		IMAGINE_CORE_ASSERT(location != -1, "Cannot find union with the given name");
		glUniform1i(location, (int)value);
	}
	void Shader::SetUniform(const std::string& name, int value) {
		GLint location = glGetUniformLocation(_ProgramID, name.c_str());
		IMAGINE_CORE_ASSERT(location != -1, "Cannot find union with the given name");
		glUniform1i(location, value);
	}
	void Shader::SetUniform(const std::string& name, float value) {
		GLint location = glGetUniformLocation(_ProgramID, name.c_str());
		IMAGINE_CORE_ASSERT(location != -1, "Cannot find union with the given name");
		glUniform1f(location, value);
	}
	void Shader::SetUniform(const std::string& name, glm::vec2 value) {
		GLint location = glGetUniformLocation(_ProgramID, name.c_str());
		IMAGINE_CORE_ASSERT(location != -1, "Cannot find union with the given name");
		glUniform2f(location, value.x, value.y);
	}
	void Shader::SetUniform(const std::string& name, glm::vec3 value) {
		GLint location = glGetUniformLocation(_ProgramID, name.c_str());
		IMAGINE_CORE_ASSERT(location != -1, "Cannot find union with the given name");
		glUniform3f(location, value.x, value.y, value.z);
	}
	void Shader::SetUniform(const std::string& name, glm::vec4 value) {
		GLint location = glGetUniformLocation(_ProgramID, name.c_str());
		IMAGINE_CORE_ASSERT(location != -1, "Cannot find union with the given name");
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}
	void Shader::SetUniform(const std::string& name, glm::mat4 value) {
		GLint location = glGetUniformLocation(_ProgramID, name.c_str());
		IMAGINE_CORE_ASSERT(location != -1, "Cannot find union with the given name");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}
}