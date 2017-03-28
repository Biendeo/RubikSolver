#pragma once

#include <array>

#include "Constants.h"

class Face {
	public:
	Face();
	Face(Color color);
	Face(std::initializer_list<Color> colors);
	Face(std::array<Color, CubeSize * CubeSize> colors);
	~Face();

	bool operator==(const Face& otherFace) const;
	bool operator!=(const Face& otherFace) const;

	Color GetColor(int space) const;
	void SetColor(int space, Color color);

	bool OneColor() const;

	private:
	// The array goes:
	// 012
	// 345
	// 678
	// Or similar if the cube size is not 3.
	std::array<Color, CubeSize * CubeSize> colors;
};