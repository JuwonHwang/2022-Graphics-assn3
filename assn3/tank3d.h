#pragma once

#include "sprite3d.h"
#include "ground.h"
#include "bomb3D.h"
#include "util.h"

enum TANKSTATUS {
	FORWARD,
	BACKWARD,
	TURNRIGHT,
	TURNLEFT,
	UPPERRIGHT,
	UPPERLEFT,
	BARRELUP,
	BARRELDOWN,
	POWERUP,
	POWERDOWN,
	SHOOT
};

class Tank3D : public Sprite3D {
private:
	Sprite3D* upperbody;
	Sprite3D* barrel;
	std::vector<Sprite3D*> bombs;
	std::vector<Sprite3D*> leftwheels;
	std::vector<Sprite3D*> rightwheels;
	Boundary boundary = Boundary(Position(25, 0, 25), Position(-25, 0, -25));
	Position upperbodyPos = Position(0.0f, 1.6f, 0.0f);
	int status = 0;
	bool is_auto = false;
	float is_recoil = 0;
	int resting = 0;
	float power = 3.0f;

public:
	Tank3D(std::string _name, Color _color, Position _position, std::vector<std::vector<Sprite3D*>*> _groups) 
		: Sprite3D(_name, _color,_position,_groups, "resource/body.obj") {
		upperbody = new Sprite3D("", _color, upperbodyPos, {}, "resource/upperbody.obj");
		barrel = new Sprite3D("", _color, Position(0.0f, 0.0f, 0.0f), {}, "resource/barrel.obj");
		upperbody->addSprite3D(barrel);
		addSprite3D(upperbody);
		//name_tag = _name;
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
		setCollisionTag("tank");
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
		getVelocity().y = 0.0f;
		getAccel().y = 0.0f;
		
		std::set<std::string> tags = getCollisionGroup();
		if (tags.find("tank") != tags.end()) {
			setVelocity({ 0,0,0 });
		}
		if (tags.find("bomb") != tags.end()) {

		}
		
		Sprite3D::update();

		if (boundary.getSmall().x >= getPosition().x) {
			setPosition({ boundary.getSmall().x, getPosition().y, getPosition().z });
		}
		if (boundary.getSmall().z >= getPosition().z) {
			setPosition({ getPosition().x, getPosition().y, boundary.getSmall().z});
		}
		if (boundary.getLarge().x <= getPosition().x) {
			setPosition({ boundary.getLarge().x, getPosition().y, getPosition().z });
		}
		if (boundary.getLarge().z <= getPosition().z) {
			setPosition({ getPosition().x, getPosition().y, boundary.getLarge().z });
		}

		if (is_recoil) {
			setAccel(getVelocity() * -0.2f);
			upperbody->setPosition(upperbodyPos + util::vibrationVec(is_recoil));
			is_recoil--;
		}
		else {
			if (resting > 0) {
				resting--;
			}
			setAccel(getVelocity() * -1.0f);
		}

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
		accelerate({ 0.2 * speed * glm::sin(getRPY().y * PI / 180), 0, 0.2 * speed * glm::cos(getRPY().y * PI / 180) });
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

	void powerUp() {
		if (power < 5) {
			power++;
		}
	}

	void powerDown() {
		if (power > 1) {
			power--;
		}
	}

	void recoil(float pow) {
		is_recoil = pow * 2;
		resting = 10;
		accelerate(getbarrelRPY() * pow * 0.02f);
	}

	float getRecoil() {
		return is_recoil;
	}

	void shoot(std::vector<Sprite3D*>* _group) {
		if (!is_recoil && !resting) {
			Bomb3D* bomb = new Bomb3D("bomb", purple, getPosition() + upperbody->getPosition() - getbarrelRPY() * 4.0f, { _group, &bombs }, -getbarrelRPY() * power / 2.0f);
			recoil(power);
		}
	}



	void autonomous() {
		int todo = rand() % 30;
		switch (todo)
		{
		case TANKSTATUS::FORWARD:
		case TANKSTATUS::BACKWARD:
		case TANKSTATUS::TURNLEFT:
		case TANKSTATUS::TURNRIGHT:
		case TANKSTATUS::UPPERLEFT:
		case TANKSTATUS::UPPERRIGHT:
		case TANKSTATUS::BARRELUP:
		case TANKSTATUS::BARRELDOWN:
		case TANKSTATUS::POWERUP:
		case TANKSTATUS::POWERDOWN:
			status = todo;
			break;
		case TANKSTATUS::SHOOT:
			shoot(&allGroups);
			status = -1;
			break;
		default:
			break;
		}
		switch (status)
		{
		case TANKSTATUS::FORWARD:
			forward(1);
			break;
		case TANKSTATUS::BACKWARD:
			forward(-1);
			break;
		case TANKSTATUS::TURNLEFT:
			turn(1);
			break;
		case TANKSTATUS::TURNRIGHT:
			turn(-1);
			break;
		case TANKSTATUS::UPPERLEFT:
			rotateHead(1);
			break;
		case TANKSTATUS::UPPERRIGHT:
			rotateHead(-1);
			break;
		case TANKSTATUS::BARRELUP:
			rotateBarrel(1);
			break;
		case TANKSTATUS::BARRELDOWN:
			rotateBarrel(-1);
			break;
		case TANKSTATUS::POWERUP:
			powerUp();
			break;
		case TANKSTATUS::POWERDOWN:
			powerDown();
			break;
		default:
			break;
		}
	}
};