#include "Imagine.h"

class MyApp : public IM::Application {
public:
	MyApp() {

	}
	~MyApp() {

	}
};

IM::Application* IM::CreateApplication() {
	return new MyApp();
}