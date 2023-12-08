#include "Imagine.h"
#include "Core/Entry.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include "Imgui/imgui.h"

#include "layers/ExampleLayer.h"
#include "layers/MyApp2D.h"

class MyApp : public IM::Application {
public:
	MyApp() {
		//PushLayer(new ExampleLayer());
		PushLayer(new MyApp2D());
	}
	~MyApp() {

	}
};

IM::Application* IM::CreateApplication() {
	return new MyApp();
}