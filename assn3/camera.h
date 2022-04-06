#pragma once

#include "sprite3d.h"

class Camera {
private:
	glm::vec3 pos = Position(0.0f, 10.0f, 10.0f);
	glm::vec3 up = Position(0.0f, 1.0f, 0.0f);

	bool isOrtho = false;

public:
	Camera() {

	}

	void View(Position target) {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		gluPerspective(45, 1, 0.01, 100);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glScalef(SCALE, SCALE, SCALE);

		gluLookAt(
			pos.x, pos.y, pos.z,
			target.x, target.y, target.z,
			up.x, up.y, up.z
		);

		
	}

};