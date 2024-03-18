#include "Imagine.h"
#include "Core/Entry.h"

#include "EditorLayer.h"

namespace IM {
	class SangSang : public Application {
	public:
		SangSang() 
			: Application("SangSang") {
			PushLayer(new EditorLayer());
		}
		~SangSang() {

		}
	};

	Application* CreateApplication() {
		return new SangSang();
	}
}

