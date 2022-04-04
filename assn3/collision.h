#pragma once

#include "spriteGroup.h"
#include "shapes.h"
#include <string>
#include <algorithm>

void printVector(Position p) {
	std::cout << "(" << p.x << ", " << p.y << ", " << p.z << ")" << std::endl;
}

void printShape(Positions s) {
	for (size_t i = 0; i < s.size(); i++) {
		std::cout << i << ": ";
		printVector(s[i]);
	}
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



bool distanceCompareDesc(Position a, Position b) {
	return glm::dot(a, a) > glm::dot(b, b);
}

bool xCompareIncr(Position a, Position b) {
	return a.x < b.x;
}

Position SimplexDir(Positions s) {
	Position d(0, 0, 0);
	for (size_t i = 0; i < s.size(); i++) {
		d += s[i];
	}
	return d;
}

// which takes a simplex sand returns the simplex on s closest to the origin,
// and a direction toward the origin normal to the new simplex. If s itself contains the origin,
// NearestSimplex accepts sand the two shapes are determined to intersect.
Positions NearestSimplex(Positions s) {
	if (s.size() < 4) {
		return s;
	}
	Position d = SimplexDir(s);
	Position a = Support(s, d);
	s.erase(std::remove(s.begin(), s.end(), a), s.end());
	return s;
}


// return true whether simplex s contains origin 
bool ContainOrigin(Positions s) {
	std::sort(s.begin(), s.end(), xCompareIncr);
	if (s.size() < 3) {
		return false;
	}

	Position a = s[0];
	Position b = s[1];
	Position c = s[2];
	Position o(0, 0, 0);

	float ABAO = glm::normalize(glm::cross(b - a, o - a)).z;
	float BCBO = glm::normalize(glm::cross(c - b, o - b)).z;
	float CACO = glm::normalize(glm::cross(a - c, o - c)).z;

	if (std::abs(ABAO + BCBO + CACO) == 3) {
		return true;
	}
	else {
		return false;
	}
}


bool GJK_intersection(Positions p, Positions q) {
	Position a = Support(p, { 1,0,0 }) - Support(q, { -1,0,0 });
	Positions s = { a };
	Position d = -a;

	while (true) {
		a = Support(p, d) - Support(q, -d);
		std::cout << "a - ";
		printVector(a);
		if (glm::dot(a, d) <= 0) {
			return false;
		}
		s.push_back(a);
		s = NearestSimplex(s);
		std::cout << "===========" << std::endl;
		std::cout << "s - " << std::endl;
		printShape(s);
		d = -SimplexDir(s);
		std::cout << "d - ";
		printVector(d);
		if (ContainOrigin(s)) {
			return true;
		}
	}

	return false;
}

bool AABB(glm::vec4 p, glm::vec4 q) {
	const int left = 0, right = 1, top = 2, bottom = 3;
	if (p[right] < q[left] || q[right] < p[left]) return false;
	if (p[top] < q[bottom] || q[top] < p[bottom]) return false;
	return true;
}

void checkAllCollision(std::vector<Sprite*> allGroup) {
	for (size_t i = 0; i < allGroup.size(); i++) {
		allGroup[i]->getCollisionGroup().clear();
	}
	for (size_t i = 0; i < allGroup.size() - 1; i++) {
		// std::cout << "=======CHECK=======" << std::endl;
		// std::cout << i << " - name: " << allGroup[i]->getName() << ", tag: " << allGroup[i]->getCollisionTag() << std::endl;
		if (allGroup[i]->getCollisionTag() == "none") {
			continue;
		}
		for (size_t j = i + 1; j < allGroup.size(); j++) {
			// std::cout << j << " - name: " << allGroup[j]->getName() << ", tag: " << allGroup[j]->getCollisionTag() << std::endl;
			if (allGroup[j]->getCollisionTag() == "none") {
				continue;
			}
			//if (GJK_intersection(allGroup[i]->getCollisionMask(Position(0, 0, 0), 0.0f), allGroup[j]->getCollisionMask(Position(0, 0, 0), 0.0f))) {
			if (AABB(allGroup[i]->getRectangle(allGroup[i]->getVelocity(), 0.0f),allGroup[j]->getRectangle(allGroup[j]->getVelocity(), 0.0f))) {
				allGroup[i]->getCollisionGroup().insert(allGroup[j]->getCollisionTag());
				allGroup[j]->getCollisionGroup().insert(allGroup[i]->getCollisionTag());
				std::cout << "(" << allGroup[i]->getName() << ": " << allGroup[i]->getCollisionTag() << ", " 
					<< allGroup[j]->getName() << ": " << allGroup[j]->getCollisionTag() << ")" << std::endl;
			}
		}
	}
	for (size_t i = 0; i < allGroup.size(); i++) {
		if (allGroup[i]->getCollisionGroup().size() > 0) {
			// std::cout << allGroup[i]->getCollisionGroup().size() << std::endl;
		}
	}
}