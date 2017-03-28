#include "Cube.h"

#include <iostream>
#include <map>
#include <queue>
#include <random>
#include <set>

#include "ActionChain.h"

// Randomly generates a cube that could possibly be solved.
Cube::Cube() {


	// This method tries to brute force randomly generate a cube, but it creates too many impossible cubes.
	/*
	std::map<Color, int> currentColorsChosen;
	std::set<Color> centerColorsChosen;

	for (int i = 0; i < 6; ++i) {
		std::array<Color, CubeSize * CubeSize> faceColors;
		for (int j = 0; j < (CubeSize * CubeSize); ++j) {
			bool successfulChoice = false;
			while (!successfulChoice) {
				Color nextColor = RandomColor(rand);
				// If the cube is odd sized, then it has centers that need to be checked.
				// If the chosen color has not yet been a center, then it can be.
				if (CubeSize % 2 == 1 && (j * 2 + 1 == CubeSize * CubeSize) && centerColorsChosen.find(nextColor) == centerColorsChosen.end()) {
					faceColors.at(j) = nextColor;
					successfulChoice = true;
				} else if (currentColorsChosen.find(nextColor) == currentColorsChosen.end() || currentColorsChosen.at(nextColor) < CubeSize * CubeSize) {
					faceColors.at(j) = nextColor;
					if (currentColorsChosen.find(nextColor) == currentColorsChosen.end()) {
						currentColorsChosen.emplace(std::make_pair(nextColor, 1));
					} else {
						++currentColorsChosen.find(nextColor);
					}
					successfulChoice = true;
				}
			}
		}
		switch (i) {
			case 0:
				front = Face(faceColors);
				break;
			case 1:
				top = Face(faceColors);
				break;
			case 2:
				left = Face(faceColors);
				break;
			case 3:
				right = Face(faceColors);
				break;
			case 4:
				bottom = Face(faceColors);
				break;
			case 5:
				back = Face(faceColors);
				break;
		}
	}
	*/


}

// Makes a cube with a preset pattern.
Cube::Cube(std::initializer_list<Color> colors) {
	if (colors.size() != CubeSize * CubeSize * 6) {
		throw std::exception("Cube being made with incorrect size.");
	}

	std::array<Color, CubeSize * CubeSize> topColors, leftColors, frontColors, rightColors, backColors, bottomColors;

	int currentColor = 0;

	for (const Color& c1 : colors) {
		if (currentColor < 1 * CubeSize * CubeSize) {
			topColors.at(currentColor % (CubeSize * CubeSize)) = c1;
		} else if (currentColor < 2 * CubeSize * CubeSize) {
			leftColors.at(currentColor % (CubeSize * CubeSize)) = c1;
		} else if (currentColor < 3 * CubeSize * CubeSize) {
			frontColors.at(currentColor % (CubeSize * CubeSize)) = c1;
		} else if (currentColor < 4 * CubeSize * CubeSize) {
			rightColors.at(currentColor % (CubeSize * CubeSize)) = c1;
		} else if (currentColor < 5 * CubeSize * CubeSize) {
			backColors.at(currentColor % (CubeSize * CubeSize)) = c1;
		} else {
			bottomColors.at(currentColor % (CubeSize * CubeSize)) = c1;
		}
		++currentColor;
	}

	top = Face(topColors);
	left = Face(leftColors);
	front = Face(frontColors);
	right = Face(rightColors);
	back = Face(backColors);
	bottom = Face(bottomColors);
}

Cube::~Cube() {

}

bool Cube::operator==(const Cube& otherCube) const {
	return (top == otherCube.top && front == otherCube.front && left == otherCube.left && back == otherCube.back && right == otherCube.right && bottom == otherCube.bottom);
}

bool Cube::operator!=(const Cube& otherCube) const {
	return !(*this == otherCube);
}

const Face& Cube::Top() const {
	return top;
}

const Face& Cube::Front() const {
	return front;
}

const Face& Cube::Bottom() const {
	return bottom;
}

const Face& Cube::Left() const {
	return left;
}

const Face& Cube::Right() const {
	return right;
}

const Face& Cube::Back() const {
	return back;
}

typedef uint64_t CubeHash;
CubeHash CubeToHash(const Cube& cube) {
	CubeHash hash = 0;
	for (int i = 0; i < ColorCount; ++i) {
		Face currentFace;
		if (i == 0) currentFace = cube.Top();
		else if (i == 1) currentFace = cube.Front();
		else if (i == 2) currentFace = cube.Right();
		else if (i == 3) currentFace = cube.Left();
		else if (i == 4) currentFace = cube.Back();
		else             currentFace = cube.Bottom();
		for (int j = 0; j < (CubeSize * CubeSize); ++j) {
			hash *= 6;
			hash += static_cast<int>(currentFace.GetColor(j));
		}
	}
	return hash;
}

// Solves a cube and returns how to solve it. The boolean argument returns whether a solution was found.
ActionChain Cube::Solve(bool& success) {


	std::queue<ActionChain> queue;
	ActionChain startingChain(*this);
	queue.push(startingChain);
	std::set<CubeHash> cubesSeen;
	cubesSeen.insert(CubeToHash(*this));

	while (!queue.empty()) {
		ActionChain nextChain = queue.front();
		queue.pop();
		for (int i = 1; i < static_cast<int>(Action::BackCClock); ++i) {
			Cube nextCube = nextChain[nextChain.Size() - 1].cube;
			nextCube.Move(static_cast<Action>(i));
			if (cubesSeen.find(CubeToHash(nextCube)) == cubesSeen.end()) {
				ActionChain newChain = nextChain;
				newChain.Act(static_cast<Action>(i));
				queue.push(newChain);
				cubesSeen.insert(CubeToHash(nextCube));
				if (nextCube.IsSolved()) {
					success = true;
					return newChain;
				}
			}
		}
	}

	success = false;
	return startingChain;
}

// Prints the cube to the console.
void Cube::PrintCube() const {
	rlutil::saveDefaultColor();

	// Start by printing some blank space, then the top face.
	for (int row = 0; row < CubeSize; ++row) {
		for (int column = 0; column < CubeSize; ++column) {
			std::cout << " ";
		}
		for (int column = 0; column < CubeSize; ++column) {
			rlutil::setColor(ColorToRlutil(top.GetColor(row * CubeSize + column)));
			std::cout << ColorToChar(top.GetColor(row * CubeSize + column));
		}
		std::cout << "\n";
	}
	// Then do each of the four middle faces.
	for (int row = 0; row < CubeSize; ++row) {
		for (int column = 0; column < CubeSize; ++column) {
			rlutil::setColor(ColorToRlutil(left.GetColor(row * CubeSize + column)));
			std::cout << ColorToChar(left.GetColor(row * CubeSize + column));
		}
		for (int column = 0; column < CubeSize; ++column) {
			rlutil::setColor(ColorToRlutil(front.GetColor(row * CubeSize + column)));
			std::cout << ColorToChar(front.GetColor(row * CubeSize + column));
		}
		for (int column = 0; column < CubeSize; ++column) {
			rlutil::setColor(ColorToRlutil(right.GetColor(row * CubeSize + column)));
			std::cout << ColorToChar(right.GetColor(row * CubeSize + column));
		}
		for (int column = 0; column < CubeSize; ++column) {
			rlutil::setColor(ColorToRlutil(back.GetColor(row * CubeSize + column)));
			std::cout << ColorToChar(back.GetColor(row * CubeSize + column));
		}
		std::cout << "\n";
	}
	// Finally, do the bottom face.
	for (int row = 0; row < CubeSize; ++row) {
		for (int column = 0; column < CubeSize; ++column) {
			std::cout << " ";
		}
		for (int column = 0; column < CubeSize; ++column) {
			rlutil::setColor(ColorToRlutil(bottom.GetColor(row * CubeSize + column)));
			std::cout << ColorToChar(bottom.GetColor(row * CubeSize + column));
		}
		std::cout << "\n";
	}

	rlutil::resetColor();
}

void Cube::Move(Action action) {
	// TODO: The rest of this.
	Cube oldCube = *this;
	switch (action) {
		case Action::None:
		default:
			break;
		case Action::TopRight:
			front.SetColor(0, oldCube.left.GetColor(0));
			front.SetColor(1, oldCube.left.GetColor(1));
			front.SetColor(2, oldCube.left.GetColor(2));
			right.SetColor(0, oldCube.front.GetColor(0));
			right.SetColor(1, oldCube.front.GetColor(1));
			right.SetColor(2, oldCube.front.GetColor(2));
			back.SetColor(0, oldCube.right.GetColor(0));
			back.SetColor(1, oldCube.right.GetColor(1));
			back.SetColor(2, oldCube.right.GetColor(2));
			left.SetColor(0, oldCube.back.GetColor(0));
			left.SetColor(1, oldCube.back.GetColor(1));
			left.SetColor(2, oldCube.back.GetColor(2));
			top.SetColor(0, oldCube.top.GetColor(2));
			top.SetColor(1, oldCube.top.GetColor(5));
			top.SetColor(2, oldCube.top.GetColor(8));
			top.SetColor(3, oldCube.top.GetColor(1));
			top.SetColor(5, oldCube.top.GetColor(7));
			top.SetColor(6, oldCube.top.GetColor(0));
			top.SetColor(7, oldCube.top.GetColor(3));
			top.SetColor(8, oldCube.top.GetColor(6));
			break;
		case Action::TopLeft:
			front.SetColor(0, oldCube.right.GetColor(0));
			front.SetColor(1, oldCube.right.GetColor(1));
			front.SetColor(2, oldCube.right.GetColor(2));
			left.SetColor(0, oldCube.front.GetColor(0));
			left.SetColor(1, oldCube.front.GetColor(1));
			left.SetColor(2, oldCube.front.GetColor(2));
			back.SetColor(0, oldCube.left.GetColor(0));
			back.SetColor(1, oldCube.left.GetColor(1));
			back.SetColor(2, oldCube.left.GetColor(2));
			right.SetColor(0, oldCube.back.GetColor(0));
			right.SetColor(1, oldCube.back.GetColor(1));
			right.SetColor(2, oldCube.back.GetColor(2));
			top.SetColor(0, oldCube.top.GetColor(6));
			top.SetColor(1, oldCube.top.GetColor(3));
			top.SetColor(2, oldCube.top.GetColor(0));
			top.SetColor(3, oldCube.top.GetColor(7));
			top.SetColor(5, oldCube.top.GetColor(1));
			top.SetColor(6, oldCube.top.GetColor(8));
			top.SetColor(7, oldCube.top.GetColor(5));
			top.SetColor(8, oldCube.top.GetColor(2));
			break;
		case Action::BottomRight:
			front.SetColor(6, oldCube.left.GetColor(6));
			front.SetColor(7, oldCube.left.GetColor(7));
			front.SetColor(8, oldCube.left.GetColor(8));
			right.SetColor(6, oldCube.front.GetColor(6));
			right.SetColor(7, oldCube.front.GetColor(7));
			right.SetColor(8, oldCube.front.GetColor(8));
			back.SetColor(6, oldCube.right.GetColor(6));
			back.SetColor(7, oldCube.right.GetColor(7));
			back.SetColor(8, oldCube.right.GetColor(8));
			left.SetColor(6, oldCube.back.GetColor(6));
			left.SetColor(7, oldCube.back.GetColor(7));
			left.SetColor(8, oldCube.back.GetColor(8));
			bottom.SetColor(0, oldCube.bottom.GetColor(6));
			bottom.SetColor(1, oldCube.bottom.GetColor(3));
			bottom.SetColor(2, oldCube.bottom.GetColor(0));
			bottom.SetColor(3, oldCube.bottom.GetColor(7));
			bottom.SetColor(5, oldCube.bottom.GetColor(1));
			bottom.SetColor(6, oldCube.bottom.GetColor(8));
			bottom.SetColor(7, oldCube.bottom.GetColor(5));
			bottom.SetColor(8, oldCube.bottom.GetColor(2));
			break;
		case Action::BottomLeft:
			front.SetColor(6, oldCube.right.GetColor(6));
			front.SetColor(7, oldCube.right.GetColor(7));
			front.SetColor(8, oldCube.right.GetColor(8));
			left.SetColor(6, oldCube.front.GetColor(6));
			left.SetColor(7, oldCube.front.GetColor(7));
			left.SetColor(8, oldCube.front.GetColor(8));
			back.SetColor(6, oldCube.left.GetColor(6));
			back.SetColor(7, oldCube.left.GetColor(7));
			back.SetColor(8, oldCube.left.GetColor(8));
			right.SetColor(6, oldCube.back.GetColor(6));
			right.SetColor(7, oldCube.back.GetColor(7));
			right.SetColor(8, oldCube.back.GetColor(8));
			bottom.SetColor(0, oldCube.bottom.GetColor(2));
			bottom.SetColor(1, oldCube.bottom.GetColor(5));
			bottom.SetColor(2, oldCube.bottom.GetColor(8));
			bottom.SetColor(3, oldCube.bottom.GetColor(1));
			bottom.SetColor(5, oldCube.bottom.GetColor(7));
			bottom.SetColor(6, oldCube.bottom.GetColor(0));
			bottom.SetColor(7, oldCube.bottom.GetColor(3));
			bottom.SetColor(8, oldCube.bottom.GetColor(6));
			break;
		case Action::LeftUp:
			top.SetColor(0, oldCube.front.GetColor(0));
			top.SetColor(3, oldCube.front.GetColor(3));
			top.SetColor(6, oldCube.front.GetColor(6));
			front.SetColor(0, oldCube.bottom.GetColor(0));
			front.SetColor(3, oldCube.bottom.GetColor(3));
			front.SetColor(6, oldCube.bottom.GetColor(6));
			bottom.SetColor(0, oldCube.back.GetColor(8));
			bottom.SetColor(3, oldCube.back.GetColor(5));
			bottom.SetColor(6, oldCube.back.GetColor(2));
			back.SetColor(2, oldCube.top.GetColor(6));
			back.SetColor(5, oldCube.top.GetColor(3));
			back.SetColor(8, oldCube.top.GetColor(0));
			left.SetColor(0, oldCube.left.GetColor(2));
			left.SetColor(1, oldCube.left.GetColor(5));
			left.SetColor(2, oldCube.left.GetColor(8));
			left.SetColor(3, oldCube.left.GetColor(1));
			left.SetColor(5, oldCube.left.GetColor(7));
			left.SetColor(6, oldCube.left.GetColor(0));
			left.SetColor(7, oldCube.left.GetColor(3));
			left.SetColor(8, oldCube.left.GetColor(6));
			break;
		case Action::LeftDown:
			top.SetColor(0, oldCube.back.GetColor(8));
			top.SetColor(3, oldCube.back.GetColor(5));
			top.SetColor(6, oldCube.back.GetColor(2));
			back.SetColor(2, oldCube.bottom.GetColor(6));
			back.SetColor(5, oldCube.bottom.GetColor(3));
			back.SetColor(8, oldCube.bottom.GetColor(0));
			bottom.SetColor(0, oldCube.front.GetColor(0));
			bottom.SetColor(3, oldCube.front.GetColor(3));
			bottom.SetColor(6, oldCube.front.GetColor(6));
			front.SetColor(0, oldCube.top.GetColor(0));
			front.SetColor(3, oldCube.top.GetColor(3));
			front.SetColor(6, oldCube.top.GetColor(6));
			left.SetColor(0, oldCube.left.GetColor(6));
			left.SetColor(1, oldCube.left.GetColor(3));
			left.SetColor(2, oldCube.left.GetColor(0));
			left.SetColor(3, oldCube.left.GetColor(7));
			left.SetColor(5, oldCube.left.GetColor(1));
			left.SetColor(6, oldCube.left.GetColor(8));
			left.SetColor(7, oldCube.left.GetColor(5));
			left.SetColor(8, oldCube.left.GetColor(2));
			break;
		case Action::RightUp:
			top.SetColor(2, oldCube.front.GetColor(2));
			top.SetColor(5, oldCube.front.GetColor(5));
			top.SetColor(8, oldCube.front.GetColor(8));
			front.SetColor(2, oldCube.bottom.GetColor(2));
			front.SetColor(5, oldCube.bottom.GetColor(5));
			front.SetColor(8, oldCube.bottom.GetColor(8));
			bottom.SetColor(2, oldCube.back.GetColor(6));
			bottom.SetColor(5, oldCube.back.GetColor(3));
			bottom.SetColor(8, oldCube.back.GetColor(0));
			back.SetColor(0, oldCube.top.GetColor(8));
			back.SetColor(3, oldCube.top.GetColor(5));
			back.SetColor(6, oldCube.top.GetColor(2));
			right.SetColor(0, oldCube.right.GetColor(6));
			right.SetColor(1, oldCube.right.GetColor(3));
			right.SetColor(2, oldCube.right.GetColor(0));
			right.SetColor(3, oldCube.right.GetColor(7));
			right.SetColor(5, oldCube.right.GetColor(1));
			right.SetColor(6, oldCube.right.GetColor(8));
			right.SetColor(7, oldCube.right.GetColor(5));
			right.SetColor(8, oldCube.right.GetColor(2));
			break;
		case Action::RightDown:
			top.SetColor(2, oldCube.back.GetColor(6));
			top.SetColor(5, oldCube.back.GetColor(3));
			top.SetColor(8, oldCube.back.GetColor(0));
			back.SetColor(0, oldCube.bottom.GetColor(8));
			back.SetColor(3, oldCube.bottom.GetColor(5));
			back.SetColor(6, oldCube.bottom.GetColor(2));
			bottom.SetColor(2, oldCube.front.GetColor(2));
			bottom.SetColor(5, oldCube.front.GetColor(5));
			bottom.SetColor(8, oldCube.front.GetColor(8));
			front.SetColor(2, oldCube.top.GetColor(2));
			front.SetColor(5, oldCube.top.GetColor(5));
			front.SetColor(8, oldCube.top.GetColor(8));
			right.SetColor(0, oldCube.right.GetColor(2));
			right.SetColor(1, oldCube.right.GetColor(5));
			right.SetColor(2, oldCube.right.GetColor(8));
			right.SetColor(3, oldCube.right.GetColor(1));
			right.SetColor(5, oldCube.right.GetColor(7));
			right.SetColor(6, oldCube.right.GetColor(0));
			right.SetColor(7, oldCube.right.GetColor(3));
			right.SetColor(8, oldCube.right.GetColor(6));
			break;
		case Action::FrontClock:
			// I'm lazy.
			break;
		case Action::FrontCClock:
			// I'm lazy.
			break;
		case Action::BackClock:
			// I'm lazy.
			break;
		case Action::BackCClock:
			// I'm lazy.
			break;
	}
}

bool Cube::IsSolved() const {
	return (top.OneColor() && front.OneColor() && left.OneColor() && back.OneColor() && right.OneColor() && bottom.OneColor());
}