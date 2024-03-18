#include "Imagine.h"
#include "Core/Entry.h"

#include "ExampleLayer.h"
#include "MyApp2D.h"

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