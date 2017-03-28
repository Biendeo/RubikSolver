#pragma once

#include <random>

#include "rlutil.h"

static const int CubeSize = 3;
static const int ColorCount = 6;

enum class Color : char {
	White,
	Blue,
	Green,
	Yellow,
	Red,
	Orange
};

enum class Action : char {
	None,
	TopRight,
	TopLeft,
	BottomRight,
	BottomLeft,
	LeftUp,
	LeftDown,
	RightUp,
	RightDown,
	FrontClock,
	FrontCClock,
	BackClock,
	BackCClock
};

inline Color RandomColor(std::default_random_engine& rand) {
	unsigned int next = rand();
	switch (next % ColorCount) {
		case 0:
		default:
			return Color::White;
		case 1:
			return Color::Blue;
		case 2:
			return Color::Green;
		case 3:
			return Color::Yellow;
		case 4:
			return Color::Red;
		case 5:
			return Color::Orange;
	}
}

inline char ColorToChar(const Color& color) {
	switch (color) {
		case Color::White:
			return 'W';
		case Color::Blue:
			return 'B';
		case Color::Green:
			return 'G';
		case Color::Yellow:
			return 'Y';
		case Color::Red:
			return 'R';
		case Color::Orange:
			return 'O';
		default:
			return ' ';
	}
}

inline char ColorToRlutil(const Color& color) {
	switch (color) {
		case Color::White:
			return rlutil::WHITE;
		case Color::Blue:
			return rlutil::LIGHTBLUE;
		case Color::Green:
			return rlutil::LIGHTGREEN;
		case Color::Yellow:
			return rlutil::YELLOW;
		case Color::Red:
			return rlutil::LIGHTRED;
		case Color::Orange:
			return rlutil::BROWN;
		default:
			return rlutil::GREY;
	}
}