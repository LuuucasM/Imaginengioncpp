#include "impch.h"

#include "Shader.h"

#include "glad/glad.h"

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
		_RendererID = glCreateProgram();

		// Attach our shaders to our program
		glAttachShader(_RendererID, vertexShader);
		glAttachShader(_RendererID, fragmentShader);

		// Link our program
		glLinkProgram(_RendererID);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(_RendererID, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(_RendererID, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(_RendererID);
			// Don't leak shaders either.
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			// Use the infoLog as you see fit.
			//IMAGINE_CORE_ASSERT(false, "Shader Linking failure!\n {0}", infoLog.data());
			// In this simple program, we'll just leave
			return;
		}

		// Always detach shaders after a successful link.
		glDetachShader(_RendererID, vertexShader);
		glDetachShader(_RendererID, fragmentShader);
	}
	Shader::~Shader() {
		glDeleteProgram(_RendererID);
	}

	void Shader::Bind() const {
		glUseProgram(_RendererID);
	}
	void Shader::Unbind() const {
		glUseProgram(0);
	}
}