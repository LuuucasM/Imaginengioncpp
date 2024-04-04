#pragma once

#include "Event.h"

namespace IM {
	class SceneChangeEvent : public Event {
	public:
		SceneChangeEvent() = default;


		static EventType GetStaticType() { return EventType::ET_SceneChange; }
		EventType GetEventType() const override { return GetStaticType(); }
		const char* GetName() const { return "SceneChange"; }
		int GetCategoryFlags() const override { return EC_Editor; }
	};
}