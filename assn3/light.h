#pragma once

#include "sprite3d.h"

class Light: public Sprite3D {
private:
	std::string lightTag = "";

public:
	Light(std::string _name, std::string _lightTag, Color _color, Position _position, std::vector<std::vector<Sprite3D*>*> _groups) : Sprite3D(_name, _color, _position, _groups, "") {
		lightTag = _lightTag;
	}

	virtual void draw3d() {
		// remove draw
	}

	void setLightTag(std::string _lightTag) {
		lightTag = _lightTag;
	}

	std::string getLightTag() {
		return lightTag;
	}

	virtual glm::vec3 getLight() = 0;
};

class DirectionalLight : public Light {
private:

public:
	DirectionalLight(std::string _name, Color _color, Position _position, std::vector<std::vector<Sprite3D*>*> _groups) : Light(_name, "d", _color, _position, _groups) {

	}

	virtual glm::vec3 getLight() {
		glm::mat4 t = glm::mat4(1.0f);
		t = glm::rotate(t, getRPY().y * PI / 180, glm::vec3(0.0f, 1.0f, 0.0f));
		t = glm::rotate(t, getRPY().z * PI / 180, glm::vec3(0.0f, 0.0f, 1.0f));
		t = glm::rotate(t, getRPY().x * PI / 180, glm::vec3(1.0f, 0.0f, 0.0f));
		glm::vec3 lightVec = t * glm::vec4(getPosition(),1.0f);
		return lightVec;
	}
};

class Sun : public DirectionalLight {
private:
	glm::vec3 rotAxis = glm::vec3(0.0f, 1.0f, 1.0f);
	float angle = 0.0f;

public:
	Sun() : DirectionalLight("sun", Color(1.0f, 1.0f, 1.0f), Position(0.0f, 1.0f, -1.0f) * 100.0f, {}) {

	}
	
	virtual void update() {
		angle += 10.0f;
		if (angle > 360.0f) {
			angle -= 360.0f;
		}
	}

	virtual glm::vec3 getLight() {
		glm::mat4 t = glm::mat4(1.0f);
		t = glm::rotate(t, angle * PI / 180, rotAxis);
		glm::vec3 lightVec = t * glm::vec4(getPosition(), 1.0f);
		return lightVec;
	}
};

class PointLight : public Light {
private:

public:
	PointLight(std::string _name, Color _color, Position _position, std::vector<std::vector<Sprite3D*>*> _groups) : Light(_name, "p", _color, _position, _groups) {

	}



	virtual glm::vec3 getLight() {

	}
};
