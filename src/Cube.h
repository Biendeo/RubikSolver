#pragma once

#include "Constants.h"
#include "Face.h"

class ActionChain;

class Cube {
	public:
	Cube();
	Cube(std::initializer_list<Color> colors);
	~Cube();

	bool operator==(const Cube& otherCube) const;
	bool operator!=(const Cube& otherCube) const;

	const Face& Top() const;
	const Face& Front() const;
	const Face& Bottom() const;
	const Face& Left() const;
	const Face& Right() const;
	const Face& Back() const;

	ActionChain Solve(bool& success);

	void PrintCube() const;

	void Move(Action action);

	bool IsSolved() const;

	private:
	Face top;
	Face front;
	Face bottom;
	Face left;
	Face right;
	Face back;
};
