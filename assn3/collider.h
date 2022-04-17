#pragma once
#include "sprite.h"

class Collider {
private:
	glm::vec3 center;
	glm::vec3 maxbox;
	std::vector<glm::vec3> normal;
public:
	Collider() {

	}

	Collider(glm::vec3 _center, glm::vec3 _maxbox, std::vector<glm::vec3>& _normal) {
		center = _center;
		maxbox = _maxbox;
		normal = _normal;
	}

	glm::vec3& getCenter() {
		return center;
	}

	glm::vec3& getMaxbox() {
		return maxbox;
	}

	std::vector<glm::vec3>& getNormals() {
		return normal;
	}
};