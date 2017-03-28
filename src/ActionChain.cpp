#include "ActionChain.h"

ActionChain::ActionChain(const Cube& startingCube) {
	states.emplace_back(startingCube, Action::None);
}

ActionChain::~ActionChain() {

}

const State ActionChain::operator[](int i) const {
	return states.at(i);
}

size_t ActionChain::Size() const {
	return states.size();
}

void ActionChain::Act(Action action) {
	Cube nextMove = states.back().cube;
	nextMove.Move(action);
	states.push_back(State(nextMove, action));
}