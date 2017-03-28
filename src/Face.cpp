#include "Face.h"

Face::Face() : Face::Face(Color::White) {}

Face::Face(Color color) {
	for (Color &c : colors) {
		c = color;
	}
}

Face::Face(std::initializer_list<Color> colors) {
	if (colors.size() != CubeSize * CubeSize) {
		throw std::exception("Cube was made with wrong size.");
	}

	for (const Color &c1 : colors) {
		for (Color &c2 : this->colors) {
			c2 = c1;
		}
	}
}

Face::Face(std::array<Color, CubeSize * CubeSize> colors) {
	this->colors = colors;
}

Face::~Face() {

}

bool Face::operator==(const Face& otherFace) const {
	return colors == otherFace.colors;
}

bool Face::operator!=(const Face& otherFace) const {
	return !(*this == otherFace);
}

Color Face::GetColor(int space) const {
	return colors.at(space);
}

void Face::SetColor(int space, Color color) {
	colors.at(space) = color;
}

bool Face::OneColor() const {
	Color firstColor = colors.at(0);
	for (int i = 1; i < CubeSize * CubeSize; ++i) {
		if (colors.at(i) != firstColor) {
			return false;
		}
	}
	return true;
}