#pragma once

#include "sprite3d.h"
#include "ground.h"
#include "bomb3D.h"

class Tank3D : public Sprite3D {
private:
	Sprite3D* upperbody;
	Sprite3D* barrel;
	std::vector<Sprite3D*> bombs;
	std::vector<Sprite3D*> leftwheels;
	std::vector<Sprite3D*> rightwheels;
	Boundary boundary = Boundary(Position(-40, -40, 0), Position(40, 40, 0));
	int status = 0;
	bool is_auto = false;
	float recoil = 0;
public:
	Tank3D(std::string _name, Color _color, Position _position, std::vector<std::vector<Sprite3D*>*> _groups) 
		: Sprite3D(_name, _color,_position,_groups, "") {
		upperbody = new Sprite3D("", _color, Position(0.0f, 1.5f, 0.0f), {}, "resource/upperbody.obj");
		Sprite3D* lowerbody = new Sprite3D("", _color, Position(), {}, "resource/body.obj");
		barrel = new Sprite3D("", _color, Position(0.0f, 0.0f, 0.0f), {}, "resource/barrel.obj");
		upperbody->addSprite3D(barrel);
		addSprite3D(upperbody);
		addSprite3D(lowerbody);
		for (int i = 0; i < 6; i++) {
			Sprite3D* wheel = new Sprite3D("", _color, Position(2.0, -1.0f, -2.5f + i), {}, "resource/wheel.obj");
			leftwheels.push_back(wheel);
			addSprite3D(wheel);
		}
		for (int i = 0; i < 6; i++) {
			Sprite3D* wheel = new Sprite3D("", _color, Position(-2.0f, -1.0f, -2.5f + i), {}, "resource/wheel.obj");
			rightwheels.push_back(wheel);
			addSprite3D(wheel);
		}
		
		move(Position(0, 2, 0));
	}

	void setAuto(bool a) {
		is_auto = a;
	}

	virtual void update() {
		std::vector<Sprite3D*>::iterator i = bombs.begin();
		while (i != bombs.end()) {
			if (*i == NULL) {
				i = bombs.erase(i);
			}
			else {
				i++;
			}
		}
		if (is_auto) {
			autonomous();
		}
		Sprite3D::update();
		setVelocity({ 0,0,0 });
	}

	void turnLeftWheels(float speed) {
		for (size_t i = 0; i < leftwheels.size(); i++) {
			leftwheels[i]->rotate(glm::vec3(speed*30, 0, 0));
		}
	}
	
	void turnRightWheels(float speed) {
		for (size_t i = 0; i < leftwheels.size(); i++) {
			rightwheels[i]->rotate(glm::vec3(speed*30, 0, 0));
		}
	}

	void turn(float speed) {
		turnLeftWheels(-speed);
		turnRightWheels(speed);
		rotate(glm::vec3(0, speed * 2, 0));
	}

	void forward(float speed) {
		setVelocity({ 0.2 * speed * glm::sin(getRPY().y * PI / 180), 0, 0.2 * speed * glm::cos(getRPY().y * PI / 180) });
		turnLeftWheels(speed);
		turnRightWheels(speed);
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

	glm::vec3 getbarrelRPY() {
		Transform transform = Transform(1.0f);
		transform = glm::rotate(transform, (getRPY().y + upperbody->getRPY().y) * PI / 180, glm::vec3(0.0f, 1.0f, 0.0f));
		transform = glm::rotate(transform, barrel->getRPY().x * PI / 180, glm::vec3(1.0f, 0.0f, 0.0f));
		glm::vec4 tip = transform * glm::vec4(0, 0, 1, 1);
		return tip;
	}


	void shoot(std::vector<Sprite3D*>* _group) {
		if (bombs.size() < 1) {
			Bomb3D* bomb = new Bomb3D("bomb", purple, getPosition() + upperbody->getPosition() - getbarrelRPY() * 4.0f, { _group, &bombs }, -getbarrelRPY() * 1.0f);
		}
	}


	void autonomous() {
		int todo = rand() % 25;
		switch (todo)
		{
		case 0:
		case 1:
		case 2:
		case 3:
			status = todo;
			break;
		case 4: // shoot
			shoot(&allGroups);
			break;
		default:
			break;
		}
		switch (status)
		{
		case 0:
			forward(-1);
			break;
		case 1:
			forward(1);
			break;
		case 2:
			turn(1);
			break;
		case 3:
			turn(-1);
			break;
		default:
			break;
		}
	}
};