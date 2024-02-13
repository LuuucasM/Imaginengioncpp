#pragma once

#include "Renderer/Shader.h"


//TODO: REMOVE !
typedef unsigned int GLenum;

namespace IM {
	class OpenGLShader : public Shader {
	public:
		OpenGLShader(const std::string& filepath);
		virtual ~OpenGLShader();

		void Bind() const override;
		void Unbind() const override;

		void SetValue(const std::string& name, bool value) override;
		void SetValue(const std::string& name, int value) override;
		void SetValue(const std::string& name, int* values, uint32_t count) override;
		void SetValue(const std::string& name, float value) override;
		void SetValue(const std::string& name, glm::vec2 value) override;
		void SetValue(const std::string& name, glm::vec3 value) override;
		void SetValue(const std::string& name, glm::vec4 value) override;
		void SetValue(const std::string& name, glm::mat3 value) override;
		void SetValue(const std::string& name, glm::mat4 value) override;
	protected:
		void CreateLayout(const std::string& vertexShader) override;
		void DiscoverUniforms() override;
	private:
		std::string ReadFile(const std::string& filepath);
		std::unordered_map<GLenum, std::string> PreProcess(std::string& source);
		bool Compile(std::unordered_map<GLenum, std::string> shaderSources);
	};
}