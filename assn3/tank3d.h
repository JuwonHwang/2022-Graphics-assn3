#pragma once

#include "sprite3d.h"

class Tank3D : public Sprite3D {
private:
	Sprite3D* upperbody;
	Sprite3D* barrel;
	std::vector<Sprite3D*> leftwheels;
	std::vector<Sprite3D*> rightwheels;
public:
	Tank3D(std::string _name, Color _color, Position _position, std::vector<std::vector<Sprite3D*>*> _groups, std::string path) 
		: Sprite3D(_name, _color,_position,_groups, path) {
		upperbody = new Sprite3D("", _color, Position(0.0f, 1.5f, 0.0f), {}, "resource/upperbody.obj");
		Sprite3D* lowerbody = new Sprite3D("", _color, Position(), {}, "resource/body.obj");
		barrel = new Sprite3D("", _color, Position(0.0f, 0.0f, 0.0f), {}, "resource/barrel.obj");
		upperbody->addSprite3D(barrel);
		addSprite3D(upperbody);
		addSprite3D(lowerbody);
		for (int i = 0; i < 6; i++) {
			Sprite3D* wheel = new Sprite3D("", _color, Position(1.5f, -1.0f, -2.5f + i), {}, "resource/wheel.obj");
			leftwheels.push_back(wheel);
			addSprite3D(wheel);
		}
		for (int i = 0; i < 6; i++) {
			Sprite3D* wheel = new Sprite3D("", _color, Position(-1.5f, -1.0f, -2.5f + i), {}, "resource/wheel.obj");
			rightwheels.push_back(wheel);
			addSprite3D(wheel);
		}
		
		move(Position(0, 2, 0));
	}

	void forward(float speed) {
		move({ speed * glm::sin(getRPY().y * PI / 180), 0, speed * glm::cos(getRPY().y * PI / 180) });
	}

	void rotateHead(float speed) {
		float next = upperbody->getRPY().y + speed;
		if (next <= 90 && next >= -90) {
			upperbody->rotate(glm::vec3(0, speed, 0));
		}
	}

	void rotateBarrel(float speed) {
		float next = barrel->getRPY().x + speed;
		if (next <= 30 && next >= 0) {
			barrel->rotate(glm::vec3(speed, 0, 0));
		}
	}
};