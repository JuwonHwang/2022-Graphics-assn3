#pragma once
#include "util.h"
#include "sprite3d.h"
#include <string>
#include <algorithm>

class Simplex {
private:
	std::vector<glm::vec3> m_points;
	unsigned m_size;
public:
	Simplex() : m_points(), m_size(0) 
	{}

	Simplex& operator=(std::initializer_list<glm::vec3> list) {
		for (auto v = list.begin(); v != list.end(); v++) {
			m_points[std::distance(list.begin(), v)] = *v;
		}
		m_size = list.size();

		return *this;
	}


	void push_front(glm::vec3 point) {
		std::vector<glm::vec3> points = { point };
		for (size_t i = 0; i < std::min(size_t(3), m_points.size()); i++) {
			points.push_back(m_points[i]);
		}
		m_points = points;
		m_size = std::min(m_size + 1, 4u);
	}

	glm::vec3& operator[](unsigned i) { return m_points[i]; }
	unsigned size() const { return m_size; }

	auto begin() const { return m_points.begin(); }
	auto end() const { return m_points.end() - (4 - m_size); }

};

void printVector(const Position& p) {
	std::cout << "(" << p.x << ", " << p.y << ", " << p.z << ")" << std::endl;
}

void printShape(const Positions& s) {
	for (size_t i = 0; i < s.size(); i++) {
		std::cout << i << ": ";
		printVector(s[i]);
	}
}

// which returns the point on shape 
// which has the highest dot product with dir
Position Support(const Positions& vertices, const Position& dir) {
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

bool Line(
	Simplex& points,
	glm::vec3& direction)
{
	glm::vec3 a = points[0];
	glm::vec3 b = points[1];

	glm::vec3 ab = b - a;
	glm::vec3 ao = -a;

	if (util::SameDirection(ab, ao)) {
		direction = glm::cross(glm::cross(ab,ao),ab);
	}

	else {
		points = { a };
		direction = ao;
	}

	return false;
}

bool Triangle(
	Simplex& points,
	glm::vec3& direction)
{
	glm::vec3 a = points[0];
	glm::vec3 b = points[1];
	glm::vec3 c = points[2];

	glm::vec3 ab = b - a;
	glm::vec3 ac = c - a;
	glm::vec3 ao = -a;

	glm::vec3 abc = glm::cross(ab,ac);

	if (util::SameDirection(glm::cross(abc,ac), ao)) {
		if (util::SameDirection(ac, ao)) {
			points = { a, c };
			direction = glm::cross(glm::cross(ac, ao), ac);
		}

		else {
			return Line(points = { a, b }, direction);
		}
	}

	else {
		if (util::SameDirection(glm::cross(ab, abc), ao)) {
			return Line(points = { a, b }, direction);
		}

		else {
			if (util::SameDirection(abc, ao)) {
				direction = abc;
			}

			else {
				points = { a, c, b };
				direction = -abc;
			}
		}
	}

	return false;
}

bool Tetrahedron(
	Simplex& points,
	glm::vec3& direction)
{
	glm::vec3 a = points[0];
	glm::vec3 b = points[1];
	glm::vec3 c = points[2];
	glm::vec3 d = points[3];

	glm::vec3 ab = b - a;
	glm::vec3 ac = c - a;
	glm::vec3 ad = d - a;
	glm::vec3 ao = -a;

	glm::vec3 abc = glm::cross(ab, ac);
	glm::vec3 acd = glm::cross(ac, ad);
	glm::vec3 adb = glm::cross(ad, ab);

	if (util::SameDirection(abc, ao)) {
		return Triangle(points = { a, b, c }, direction);
	}

	if (util::SameDirection(acd, ao)) {
		return Triangle(points = { a, c, d }, direction);
	}

	if (util::SameDirection(adb, ao)) {
		return Triangle(points = { a, d, b }, direction);
	}

	return true;
}

bool NextSimplex(Simplex& s, glm::vec3& d) {
	switch (s.size()) {
	case 2: return Line(s, d);
	case 3: return Triangle(s, d);
	case 4: return Tetrahedron(s, d);
	}

	// never should be here
	return false;
}


bool GJK_intersection(Positions p, Positions q) {
	Position a = Support(p, { 1,0,0 }) - Support(q, { -1,0,0 });
	Simplex s;
	s.push_front(a);
	Position d = -a;

	while (true) {
		a = Support(p, d) - Support(q, -d);
		if (glm::dot(a, d) <= 0) {
			return false;
		}
		s.push_front(a);
		if (NextSimplex(s, d)) {
			return true;
		}
	}
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
			if (GJK_intersection(allGroup[i]->getCollider(Transform(0.0f)), allGroup[j]->getCollider(Transform(0.0f)))) {
			// if (AABB(allGroup[i]->getRectangle(allGroup[i]->getVelocity(), 0.0f), allGroup[j]->getRectangle(allGroup[j]->getVelocity(), 0.0f))) {
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