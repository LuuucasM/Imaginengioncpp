#include "Imagine.h"

class RockPaperScissors : public IM::Application {
public:
	RockPaperScissors() {

	}
	~RockPaperScissors() {

	}
};

IM::Application* IM::CreateApplication() {
	return new RockPaperScissors();
}