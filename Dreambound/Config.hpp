#ifndef CONFIG_HPP
#define CONFIG_HPP

enum GameEvent {
	BeginStep = 0x01,
	Step = 0x02,
	EndStep = 0x04,
	Render = 0x08,
	OtherEvent1 = 0x10,
	OtherEvent2 = 0x20

	// 0x01 ==   1 == "00000001"
	// 0x02 ==   2 == "00000010"
	// 0x04 ==   4 == "00000100"
	// 0x08 ==   8 == "00001000"
	// 0x10 ==  16 == "00010000"
	// 0x20 ==  32 == "00100000"
	// 0x40 ==  64 == "01000000"
	// 0x80 == 128 == "10000000"
};

#define SCREEN_WIDTH 1080
#define SCREEN_HEIGHT 720

#define PI 3.14159265

#endif