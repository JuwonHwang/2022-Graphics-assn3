#pragma once

#include "sprite3d.h"

class Light: public Sprite3D {
private:
	std::string lightTag = "";
public:
	Light(std::string _name,std::string _lightTag, Color _color, Position _position, std::vector<std::vector<Sprite3D*>*> _groups) : Sprite3D(_name, _color, _position, _groups, "") {
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
};

class DirectionalLight : public Light {
private:

public:
	DirectionalLight(std::string _name, Color _color, Position _position, std::vector<std::vector<Sprite3D*>*> _groups) : Light(_name, "d", _color, _position, _groups) {

	}

	

	virtual void getLight() {

	}
};

class PointLight : public Light {
private:

public:
	PointLight(std::string _name, Color _color, Position _position, std::vector<std::vector<Sprite3D*>*> _groups) : Light(_name, "p", _color, _position, _groups) {

	}



	virtual void getLight() {

	}
};