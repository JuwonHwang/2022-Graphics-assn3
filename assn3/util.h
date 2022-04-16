#pragma once

#include "sprite.h"

namespace util {
	float norm(glm::vec3 vec) {
		return sqrtf(glm::dot(vec, vec));
	}

	bool friction(glm::vec3& vec, float mu) {
		if (1 < mu) {
			mu = 1;
		}
		else if (mu < 0) {
			mu = 0;
		}
		vec = vec - vec * mu;
		if (norm(vec)<0.2) {
			vec = glm::vec3(0, 0, 0);
			return false;
		}
		return true;
	}

	glm::vec3 vibrationVec(float mag) {
		glm::vec3 vec = {
				0.01f * mag * ((rand() * 2.0f / RAND_MAX) - 1),
				0.01f * mag * ((rand() * 2.0f / RAND_MAX) - 1),
				0.01f * mag * ((rand() * 2.0f / RAND_MAX) - 1) };
		return vec;
	}
}