#pragma once

#include "sprite3d.h"
#include "util.h"
#include <string>
#include <algorithm>

class Simplex {
private:
	std::vector<glm::vec3> vertices;
public:
	Simplex() {

	}


	bool add(glm::vec3 v) {
		for (size_t i = 0; i < vertices.size(); i++) {
			if (vertices[i] == v) {
				return false;
			}
		}
		if (vertices.size() < 4) {
			vertices.push_back(v);
		}
		else {
			vertices.erase(vertices.begin());
			vertices.push_back(v);
		}
		return true;
	}

	void remove(int at) {
		vertices.erase(vertices.begin() + at);
	}

	std::vector<glm::vec3>& getVertices() {
		return vertices;
	}

	glm::vec3& operator[](const int i) {
		return vertices[i];
	}

};

// which returns the point on shape 
// which has the highest dot product with dir
glm::vec3 FarestPoint(std::vector<glm::vec3> vertices, glm::vec3 dir) {
	float highest = 0;
	size_t index = 0;
	for (size_t i = 0; i < vertices.size(); i++) {
		float temp = glm::dot(vertices[i], dir);
		if (temp > highest) {
			highest = temp;
			index = i;
		}
	}
	return vertices[index];
}

glm::vec3 Support(std::vector<glm::vec3> a, std::vector<glm::vec3> b, glm::vec3 dir) {
	return FarestPoint(a, dir) - FarestPoint(b, -dir);
}

Position SimplexDir(Positions s) {
	Position d(0, 0, 0);
	for (size_t i = 0; i < s.size(); i++) {
		d += s[i];
	}
	return glm::normalize(d);
}

// return true whether simplex s contains origin 
bool ContainOrigin(std::vector<glm::vec3> s) {

	glm::vec3 a, b, c, d,
		AB, BC,
		AC, CD,
		AD, DB,
		CB, BD;
	a = s[0];
	b = s[1];
	c = s[2];
	d = s[3];

	glm::vec3 ABC, ACD, ADB, CBD;
	glm::vec3 ABCO, ACDO, ADBO, CBDO;

	AB = b - a;
	BC = c - b;
	AC = c - a;
	CD = d - c;
	AD = d - a;
	DB = b - d;
	CB = -BC;
	BD = -DB;

	ABC = glm::cross(AB, BC);
	ACD = glm::cross(AC, CD);
	ADB = glm::cross(AD, DB);
	CBD = glm::cross(CB, BD);

	ABCO = a + b + c;
	ACDO = a + c + d;
	ADBO = a + d + b;
	CBDO = c + b + d;

	bool t1, t2, t3, t4;
	t1 = glm::dot(ABC, ABCO) > 0;
	t2 = glm::dot(ACD, ACDO) > 0;
	t3 = glm::dot(ADB, ADBO) > 0;
	t4 = glm::dot(CBD, CBDO) > 0;

	if (t1 == t2 && t2 == t3 && t3 == t4) {
		return true;
	}
	return false;
}


bool GJK_intersection(Positions p, Positions q) {
	Simplex s;
	Position a = Support(p, q, { 0,1,0 });
	s.add(Support(p, q, { -1,-1,-1 }));
	s.add(Support(p, q, { -1,-1,1 }));
	s.add(Support(p, q, { 1,-1,0 }));
	Position d = -a;

	while (true) {
		a = Support(p, q, -d);
		s.add(a);
		if (glm::dot(d, a) < 0) {
			std::cout << "hi" << std::endl;
			return false;
		}
		d = -a;
		if (ContainOrigin(s.getVertices())) {
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

void checkAllCollision(std::vector<Sprite3D*> allGroup) {
	for (size_t i = 0; i < allGroup.size(); i++) {
		allGroup[i]->getCollisionGroup().clear();
	}
	for (size_t i = 0; i < allGroup.size() - 1; i++) {
		if (allGroup[i]->getCollisionTag() == "none") {
			continue;
		}
		std::cout << "=======CHECK=======" << std::endl;
		std::cout << i << " - name: " << allGroup[i]->getName() << ", tag: " << allGroup[i]->getCollisionTag() << std::endl;
		for (size_t j = i + 1; j < allGroup.size(); j++) {
			std::cout << j << " - name: " << allGroup[j]->getName() << ", tag: " << allGroup[j]->getCollisionTag() << std::endl;
			if (allGroup[j]->getCollisionTag() == "none") {
				continue;
			}
			if (GJK_intersection(allGroup[i]->getCollider(Transform(1.0f)), allGroup[j]->getCollider(Transform(1.0f)))) {
				//if (AABB(allGroup[i]->getRectangle(allGroup[i]->getVelocity(), 0.0f), allGroup[j]->getRectangle(allGroup[j]->getVelocity(), 0.0f))) {
				allGroup[i]->getCollisionGroup().insert(allGroup[j]->getCollisionTag());
				allGroup[j]->getCollisionGroup().insert(allGroup[i]->getCollisionTag());
				std::cout << "(" << allGroup[i]->getName() << ": " << allGroup[i]->getCollisionTag() << ", "
					<< allGroup[j]->getName() << ": " << allGroup[j]->getCollisionTag() << ")" << std::endl;
				std::cout << "hi" << std::endl;
			}
		}
	}
	for (size_t i = 0; i < allGroup.size(); i++) {
		if (allGroup[i]->getCollisionGroup().size() > 0) {
			std::cout << allGroup[i]->getCollisionGroup().size() << std::endl;
		}
	}
}