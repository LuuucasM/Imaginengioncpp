#pragma once
#include "Component.h"

#include "ScriptClass.h"

namespace IM {
	struct C_NativeScript : Component {
		ScriptClass* Instance = nullptr;
		ScriptClass* (*CreateScript)();
		void (*DestroyScript)(C_NativeScript*);

		template<typename T>
		void Bind() {
			CreateScript = []() { return static_cast<ScriptClass*>(new T()); };
			DestroyScript = [](C_NativeScript* nativeScript) {delete nativeScript->Instance; nativeScript->Instance = nullptr; };
		}
	};
}