#pragma once

#include "sprite3d.h"

class Boundary {
private:
	Position large;
	Position small;

public:
	Boundary() {

	}

	Boundary(Position _large, Position _small) {
		large = _large;
		small = _small;
	}

	bool check2D(Position pos) {
		if (large.x < pos.x && pos.x < small.x ||
			large.y < pos.y && pos.y < small.y) {
			return false;
		}
	}
};

class Ground : public Sprite3D {
private:
	Boundary boundary;
	glm::vec2 size;
public:
	Ground() {

	}

	Ground(std::string _name, Color _color, Position _position, std::vector<std::vector<Sprite3D*>*> _groups, std::string path,
		glm::vec2 _size)
		: Sprite3D(_name, _color, _position, _groups, path) {
		size = _size;
		for (int i = 0; i < size.x; i++) {
			for (int j = 0; j < size.y; j++) {
				Sprite3D* plane = new Sprite3D("plane" + std::to_string(i) + std::to_string(j), grey, Position((i - size.x * 0.5) * 4, 0, (j - size.y * 0.5) * 4), {}, "resource/plane.obj");
				addSprite3D(plane);
			}
		}
	}

	void setBoundary(Boundary _b) {
		boundary = _b;
	}

	Boundary& getBoundary() {
		return boundary;
	}

};