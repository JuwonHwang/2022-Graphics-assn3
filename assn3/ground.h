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
			large.z < pos.z && pos.z < small.z) {
			return false;
		}
	}

	Position getLarge() {
		return large;
	}

	Position getSmall() {
		return small;
	}
};

class Ground : public Sprite3D {
private:
	glm::vec2 size;
public:
	Ground() {

	}

	Ground(std::string _name, Color _color, Position _position, std::vector<std::vector<Sprite3D*>*> _groups, std::string path,
		glm::vec2 _size)
		: Sprite3D(_name, _color, _position, _groups, "resource/plane") {
		size = _size;
		setTexture(textures[0]);
		for (int i = 0; i < size.x; i++) {
			for (int j = 0; j < size.y; j++) {
				Sprite3D* plane = new Sprite3D("plane" + std::to_string(i) + std::to_string(j), grey, Position((i - size.x * 0.5) * 8, 0, (j - size.y * 0.5) * 8), {}, "resource/plane");
				addSprite3D(plane);
				plane->setTexture(textures[0]);
			}
		}
	}

};