#pragma once

#include "Timer.h"

#include <iostream>

namespace IM {
	struct ProfileResult {
		const char* Name;
		float Time;
	};
	template <typename Fn>
	class ProfileTimer {
	public:
		ProfileTimer(const char * name, Fn&& func)
			: _Name(name), _Func(func){
			_Timer.start();
		}
		~ProfileTimer() {
			_Timer.stop();
			_Func({ _Name, (float)_Timer.getElapsedTimeInMilliSec() });
		}

	private:
		const char* _Name;
		Fn _Func;
		Timer _Timer;
	};
}