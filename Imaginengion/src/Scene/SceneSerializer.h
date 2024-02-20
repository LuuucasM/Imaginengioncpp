#pragma once

#include "Scene.h"

namespace IM {
	class SceneSerializer
	{
	public:
		SceneSerializer(const RefPtr<Scene>& scene);

		void SerializeText(const std::string& filepath);
		void SerializeRuntime(const std::string& filepath);

		bool DeSerializeText(const std::string& filepath);
		bool DeSerializeRuntime(const std::string& filepath);
	private:
		RefPtr<Scene> _Scene;
	};
}
