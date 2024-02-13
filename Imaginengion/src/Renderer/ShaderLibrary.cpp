#include "impch.h"
#include "ShaderLibrary.h"


namespace IM {
	void ShaderLibrary::Add(const RefPtr<Shader>& shader) {

		auto& name = shader->GetName();
		IMAGINE_CORE_ASSERT(!ShaderExists(name), "Shader already in shader library!");
		_Shaders[name] = shader;
	}
	RefPtr<Shader> ShaderLibrary::Load(const std::string& filepath) {
		auto shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}
}