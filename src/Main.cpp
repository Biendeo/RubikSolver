#include <iostream>

#include "Cube.h"
#include "ActionChain.h"

int main(int argc, char* argv[]) {
	Cube randomCube;
	// This cube should be solvable.
	Cube presetCube({Color::Blue, Color::Orange, Color::White, Color::Orange, Color::Yellow, Color::Green, Color::Green, Color::White, Color::Yellow,
					Color::Red, Color::White, Color::White, Color::Yellow, Color::Green, Color::Yellow, Color::Blue, Color::Red, Color::Red,
					Color::Orange, Color::Blue, Color::Red, Color::Blue, Color::Orange, Color::White, Color::Green, Color::Orange, Color::Blue,
					Color::Green, Color::White, Color::Blue, Color::Red, Color::Blue, Color::Red, Color::Yellow, Color::Green, Color::Green,
					Color::Orange, Color::Green, Color::White, Color::Green, Color::Red, Color::Red, Color::Yellow, Color::Yellow, Color::Yellow,
					Color::White, Color::Blue, Color::Red, Color::Blue, Color::White, Color::Yellow, Color::Orange, Color::Orange, Color::Orange});

	presetCube.PrintCube();
	
	std::cout << "\n";

	Cube originalCube = presetCube;

	presetCube.Move(Action::RightUp);

	presetCube.PrintCube();

	std::cout << "\n";

	presetCube.Move(Action::RightDown);

	presetCube.PrintCube();

	std::cout << "\nBoth cubes are " << (presetCube == originalCube ? "equal" : "different") << ".\n";

	bool success;

	ActionChain chain = presetCube.Solve(success);

	std::cout << "Solving was a " << (success ? "success" : "failure") << ".";

	return 0;
}