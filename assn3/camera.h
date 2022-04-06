#pragma once

#include "sprite3d.h"

enum cmode {
	THIRD_PERSON,
	FIRST_PERSON,
	TOP_VIEW
};

class Camera {
private:
	glm::vec3 up = Position(0.0f, 1.0f, 0.0f);

	cmode mode = THIRD_PERSON;
	
	bool isOrtho = false;

public:
	Camera() {

	}

	void View(Position target, glm::vec3 rpy) {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		switch (mode)
		{
		case THIRD_PERSON:
			gluPerspective(45, 1, 0.01, 100);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			glScalef(SCALE, SCALE, SCALE);

			gluLookAt(
				target.x + 20 * glm::sin(rpy.y * PI / 180), 10 + target.y, target.z + 20 * glm::cos(rpy.y * PI / 180),
				target.x, target.y, target.z,
				up.x, up.y, up.z
			);
			break;
		case FIRST_PERSON:
			gluPerspective(45, 1, 0.01, 100);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			glScalef(SCALE, SCALE, SCALE);

			gluLookAt(
				target.x, target.y + 10, target.z,
				target.x, target.y + 10, target.z + 20,
				up.x, up.y, up.z
			);
			break;
		case TOP_VIEW:
			break;
		default:
			break;
		}

	}

	void ChangeMode() {
		switch (mode)
		{
		case THIRD_PERSON:
			mode = FIRST_PERSON;
			break;
		case FIRST_PERSON:
			mode = TOP_VIEW;
			break;
		case TOP_VIEW:
			break;
			mode = THIRD_PERSON;
		default:
			break;
		}
	}

};