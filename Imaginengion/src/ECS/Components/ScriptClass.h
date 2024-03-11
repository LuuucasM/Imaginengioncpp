#pragma once


#include "ECS/Entity.h"

namespace IM {
	class ScriptClass {
	friend class Scene;
	public:
		virtual ~ScriptClass() = default;
		
		template<typename C_Type>
		C_Type& GetComponent() {
			return _Entity.GetComponent<C_Type>();
		}

		template<typename ...Args>
		auto GetComponents() {
			return _Entity.GetComponents<Args...>();
		}

	protected:
		virtual void OnCreate() {};
		virtual void OnDestroy() {};
		virtual void OnUpdate(float dt) {};
		Entity _Entity;
	private:
		
	};
}