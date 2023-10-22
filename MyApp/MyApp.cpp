#include "Imagine.h"

class ExampleLayer : public IM::Layer {
public:
	ExampleLayer() 
		: Layer("Example") {

	}

	void OnUpdate() override {
		IMAGINE_INFO("ExampleLayer::OnUpdate");
	}

};

class MyApp : public IM::Application {
public:
	MyApp() {
		PushLayer(new ExampleLayer());
	}
	~MyApp() {

	}
};

IM::Application* IM::CreateApplication() {
	return new MyApp();
}