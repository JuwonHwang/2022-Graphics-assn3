#pragma once

#include "sprite3d.h"
#include "util.h"
#include <string>
#include <algorithm>

void printVector(Position p) {
	std::cout << "(" << p.x << ", " << p.y << ", " << p.z << ")" << std::endl;
}

// which returns the point on shape 
// which has the highest dot product with dir
Position Support(Positions vertices, Position dir) {
	float length = 0;
	size_t index = 0;
	for (size_t i = 0; i < vertices.size(); i++) {
		float temp = glm::dot(vertices[i], dir);
		if (temp > length) {
			length = temp;
			index = i;
		}
	}
	return vertices[index];
}

bool OBB_Check(Collider a, Collider b) {

	std::vector<glm::vec3> axis;

	glm::vec3 ac = a.getCenter();
	glm::vec3 bc = b.getCenter();
	glm::vec3 av = a.getMaxbox();
	glm::vec3 bv = b.getMaxbox();
	std::vector<glm::vec3> an = a.getNormals();
	std::vector<glm::vec3> bn = b.getNormals();

	bool parallel = false;

	for (size_t i = 0; i < an.size(); i++) {
		for (size_t j = 0; j < bn.size(); j++) {
			if (abs(glm::dot(an[i], bn[i])) > 0.999f) {
				parallel = true;
			}
		}
	}

	for (size_t i = 0; i < an.size(); i++) {
		axis.push_back(an[i]);
	}
	if (!parallel) {
		for (size_t i = 0; i < bn.size(); i++) {
			axis.push_back(bn[i]);
		}
		for (size_t i = 0; i < an.size(); i++) {
			for (size_t j = 0; j < bn.size(); j++) {
				if (util::norm(glm::cross(an[i], bn[i])) == 1) {
					axis.push_back(glm::cross(an[i], bn[j]));
				}
			}
		}
	}

	Position fa = av - ac;
	Position fb = av - bc;

	for (size_t k = 0; k < axis.size(); k++) {

		float acbc = glm::dot(bc - ac,axis[k]);
		float dfa = glm::dot(fa, axis[k]);
		float dfb = glm::dot(fb, axis[k]);

		std::cout << "======NEW======" << std::endl;
		printVector(axis[k]);
		std::cout << "===============" << std::endl;
		std::cout << dfa << std::endl;
		std::cout << dfb << std::endl;
		std::cout << acbc << std::endl;
		std::cout << "===============" << std::endl;
		std::cout << k << std::endl;
		std::cout << "===============" << std::endl;

		if (acbc > dfa + dfb) {
			return false;
		}
	}
	return true;

}


void checkAllCollision(std::vector<Sprite3D*> allGroup) {
	for (size_t i = 0; i < allGroup.size(); i++) {
		allGroup[i]->getCollisionGroup().clear();
	}
	for (size_t i = 0; i < allGroup.size() - 1; i++) {
		if (allGroup[i]->getCollisionTag() == "none") {
			continue;
		}
		/*std::cout << "=======CHECK=======" << std::endl;
		std::cout << i << " - name: " << allGroup[i]->getName() << ", tag: " << allGroup[i]->getCollisionTag() << std::endl;*/
		for (size_t j = i + 1; j < allGroup.size(); j++) {
			//std::cout << j << " - name: " << allGroup[j]->getName() << ", tag: " << allGroup[j]->getCollisionTag() << std::endl;
			if (allGroup[j]->getCollisionTag() == "none") {
				continue;
			}
			if (OBB_Check(allGroup[i]->getCollider(Transform(1.0f), Transform(1.0f)), allGroup[j]->getCollider(Transform(1.0f),Transform(1.0f)))) {
				allGroup[i]->getCollisionGroup().insert(allGroup[j]->getCollisionTag());
				allGroup[j]->getCollisionGroup().insert(allGroup[i]->getCollisionTag());
				std::cout << "(" << allGroup[i]->getName() << ": " << allGroup[i]->getCollisionTag() << ", "
					<< allGroup[j]->getName() << ": " << allGroup[j]->getCollisionTag() << ")" << std::endl;
			}
		}
	}
	/*for (size_t i = 0; i < allGroup.size(); i++) {
		if (allGroup[i]->getCollisionGroup().size() > 0) {
			std::cout << allGroup[i]->getCollisionGroup().size() << std::endl;
		}
	}*/
}