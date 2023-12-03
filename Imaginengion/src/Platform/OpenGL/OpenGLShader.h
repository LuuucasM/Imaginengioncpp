#pragma once

#include "Renderer/Shader.h"


//TODO: REMOVE !
typedef unsigned int GLenum;

namespace IM {
	class OpenGLShader : public Shader {
	public:
		OpenGLShader(const std::string& vertexSrc, const std::string &fragmentSrc);
		OpenGLShader(const std::string& filepath);
		virtual ~OpenGLShader();

		void Bind() const override;
		void Unbind() const override;

		void SetUniform(const std::string& name, bool value);
		void SetUniform(const std::string& name, int value);
		void SetUniform(const std::string& name, float value);
		void SetUniform(const std::string& name, glm::vec2 value);
		void SetUniform(const std::string& name, glm::vec3 value);
		void SetUniform(const std::string& name, glm::vec4 value);
		void SetUniform(const std::string& name, glm::mat3 value);
		void SetUniform(const std::string& name, glm::mat4 value);

	private:
		std::string ReadFile(const std::string& filepath);
		std::unordered_map<GLenum, std::string> PreProcess(std::string& source);
		void Compile(std::unordered_map<GLenum, std::string> shaderSources);
	private:
		uint32_t _ProgramID;
	};
}