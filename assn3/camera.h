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
				target.x + rpy.x * 2, target.y + 3.2 - rpy.y * 3, target.z + rpy.z * 2,
				target.x - rpy.x * 6, target.y + 2.2 - rpy.y * 5, target.z - rpy.z * 6,
				up.x, up.y, up.z
			);
			break;
		case TOP_VIEW:
			glOrtho(-3, 3, -3, 3, -10, 10);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			glScalef(SCALE, SCALE, SCALE);

			gluLookAt(
				0,10,0,
				0,0,0,
				0,0,-1
			);
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
			mode = THIRD_PERSON;
			break;
		default:
			break;
		}
	}

	cmode getMode() {
		return mode;
	}

};