#pragma once

namespace IM {
	using MouseCode = uint16_t;
	namespace Mouse {
		enum : MouseCode {
			ModShift = 0x0001,
			ModControl = 0x0002,
			ModAlt = 0x0004,
			ModSuper = 0x0008,
			ModCapsLock = 0x0010,
			ModNumLock = 0x0020,

			Button0 = 0,
			Button1 = 1,
			Button2 = 2,
			Button3 = 3,
			Button4 = 4,
			Button5 = 5,
			Button6 = 6,
			Button7 = 7,

			ButtonLast = Button7,
			ButtonLeft = Button0,
			ButtonRight = Button1,
			ButtonMiddle = Button2
		};
	}
}