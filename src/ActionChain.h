#pragma once

#include <vector>

#include "Cube.h"
#include "Constants.h"

struct State {
	Cube cube;
	Action previousAction;

	State(const Cube& cube, Action previousAction) {
		this->cube = cube;
		this->previousAction = previousAction;
	}
};

class ActionChain {
	public:
	ActionChain(const Cube& startingCube);
	~ActionChain();

	const State operator[](int i) const;
	size_t Size() const;
	void Act(Action action);

	private:
	std::vector<State> states;
};