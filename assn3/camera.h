#pragma once

#include "sprite3d.h"
#include "util.h"

enum class cmode {
	THIRD_PERSON,
	FIRST_PERSON,
	TOP_VIEW
};

class Camera {
private:
	
	glm::vec3 up = Position(0.0f, 1.0f, 0.0f);
	glm::vec3 eye = Position();
	glm::vec3 center = Position();

	glm::vec3 pre_up = Position(0.0f, 1.0f, 0.0f);
	glm::vec3 pre_eye = Position();
	glm::vec3 pre_center = Position();

	glm::vec3 d_up = Position();
	glm::vec3 d_eye = Position();
	glm::vec3 d_center = Position();

	glm::vec3 pre_target = Position();
	glm::vec3 pre_rpy = Position();

	float smooth = 0.5f;

	cmode mode = cmode::THIRD_PERSON;
	
	bool isOrtho = false;

public:
	Camera() {

	}

	Transform View(Position target, glm::vec3 rpy, float recoil) {


		
		glm::vec3 recoil_animation = util::vibrationVec(recoil / smooth);

		target = pre_target * smooth + target * (1 - smooth);
		rpy = pre_rpy * smooth + rpy * (1 - smooth);

		pre_target = target;
		pre_rpy = rpy;

		pre_eye = eye;
		pre_center = center;
		pre_up = up;

		Transform p;

		switch (mode)
		{
		case cmode::THIRD_PERSON:
			p = glm::perspective(45.0f, 1.0f, 0.01f, 100.0f);

			eye = {
				target.x + 30 * glm::sin(rpy.y * PI / 180),
				10 + target.y,
				target.z + 30 * glm::cos(rpy.y * PI / 180)
			};

			center = {
				target.x, target.y + 5, target.z
			};

			up = { 0,1,0 };

			eye = pre_eye * smooth + eye * (1- smooth);
			center = pre_center * smooth + center * (1 - smooth);
			up = pre_up * smooth + up * (1 - smooth);

			break;
		case cmode::FIRST_PERSON:
			p = glm::perspective(45.0f, 1.0f, 0.01f, 100.0f);

			eye = {
				target.x + rpy.x * 2,
				target.y + 3.2 - rpy.y * 3,
				target.z + rpy.z * 2 
			};

			center = {
				target.x - rpy.x * 6 + recoil_animation.x,
				target.y + 2.2 - rpy.y * 5 + recoil_animation.y, 
				target.z - rpy.z * 6 + recoil_animation.z
			};

			up = { 0,1,0 };

			eye = pre_eye * smooth + eye * (1 - smooth);
			center = pre_center * smooth + center * (1 - smooth);
			up = pre_up * smooth + up * (1 - smooth);

			break;
		case cmode::TOP_VIEW:
			p = glm::ortho(-5, 5, -5, 5, -10, 10);

			eye = {
				0,10,0
			};

			center = {
				0,0,0
			};

			up = { 0,0,-1 };

			eye = pre_eye * smooth + eye * (1 - smooth);
			center = pre_center * smooth + center * (1 - smooth);
			up = pre_up * smooth + up * (1 - smooth);

			break;
		default:
			break;
		}

		Transform v = glm::lookAt(eye, center, up);

		return p * v;
	}

	void ChangeMode() {
		switch (mode)
		{
		case cmode::THIRD_PERSON:
			mode = cmode::FIRST_PERSON;
			break;
		case cmode::FIRST_PERSON:
			mode = cmode::TOP_VIEW;
			break;
		case cmode::TOP_VIEW:
			mode = cmode::THIRD_PERSON;
			break;
		default:
			break;
		}
	}

	cmode getMode() {
		return mode;
	}

};