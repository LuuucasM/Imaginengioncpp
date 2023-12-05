#pragma once

#include <string>
#include <unordered_map>

#include "Shader.h"
#include "Core/Core.h"

namespace IM {
	class ShaderLibrary
	{
	public:
		void Add(const RefPtr<Shader>& shader);
		RefPtr<Shader> Load(const std::string& filepath);
		RefPtr<Shader> Load(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);

		inline RefPtr<Shader> Get(const std::string& name) {
			IMAGINE_CORE_ASSERT(ShaderExists(name), "Shader not found in shader library!");
			return _Shaders[name];
		}
		inline bool ShaderExists(const std::string& name){ return _Shaders.find(name) != _Shaders.end(); }
	private:
		std::unordered_map<std::string, RefPtr<Shader>> _Shaders;
	};
}