#pragma once



namespace IM {
	class ScriptClass;
	struct C_NativeScript {
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