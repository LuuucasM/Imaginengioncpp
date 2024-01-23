#pragma once

#include "Timer.h"

namespace IM {
	class TimeStep {
	public:
		TimeStep() 
			: _LastFrameTime(0.0){
		}
		void Start() { _Timer.start(); }
		//inline double GetSeconds() { return _Timer.getElapsedTimeInSec(); }
		//inline double GetMilliseconds() { return _Timer.getElapsedTimeInMilliSec(); }
		//inline double GetLastFrameTime() const { return _LastFrameTime; }
		//inline void SetLastFrameTime(double lastFrame) { _LastFrameTime = lastFrame; }
		float GetDeltaTimeInSec() {
			double time = _Timer.getElapsedTimeInSec();
			double delta_time = time - _LastFrameTime;
			_LastFrameTime = time;
			return (float)delta_time;
		}
	private:
		Timer _Timer;
		double _LastFrameTime;
	};
}