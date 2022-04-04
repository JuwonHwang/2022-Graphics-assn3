#pragma once

class Color {
private:
	float red;
	float green;
	float blue;
	float alpha;

public:
	Color() {
		red = 0.0f;
		green = 0.0f;
		blue = 0.0f;
		alpha = 0.0f;
	};

	Color(float _red, float _green, float _blue, float _alpha = 1.0f) {
		red = _red;
		green = _green;
		blue = _blue;
		alpha = _alpha;
	}

	Color(const Color& _color) {
		red = _color.red;
		green = _color.green;
		blue = _color.blue;
		alpha = _color.alpha;
	}

	float& operator [] (const int index)
	{
		if (index == 0) {
			return red;
		}
		else if (index == 1) {
			return green;
		}
		else if (index == 2) {
			return blue;
		}
		else if (index == 3) {
			return alpha;
		}
		return red;
	}
};

const Color red = Color(1.0f, 0.3f, 0.3f);
const Color yellow = Color(0.7f, 0.7f, 0.0f);
const Color green = Color(0.5f, 1.0f, 0.5f);
const Color blue = Color(0.0f, 0.0f, 0.7f);
const Color purple = Color(0.3f, 0.0f, 1.0f);