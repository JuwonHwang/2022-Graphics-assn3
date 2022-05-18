#pragma once

#include "sprite3d.h"
#include "light.h"

std::vector<PointLight*> pointLights;

class Bomb3D : public Sprite3D {
private:
    PointLight* light = 0;
public:
    Bomb3D(std::string _name, Color _color, Position _position, std::vector<std::vector<Sprite3D*>*> _groups, Position _vel)
        :Sprite3D(_name, _color, _position, _groups, "resource/ball.obj") {
        setVelocity(_vel);
        setAccel(glm::vec3(0.0f, -0.03f, 0.0f));
        setCollisionTag("bomb");
        light = new PointLight("pointlight", white, getPosition() + Position(0.0f, 2.0f, 0.0f), {});
        pointLights.push_back(light);
    }

    virtual void update() {
        Sprite3D::update();
        light->setPosition(getPosition() + Position(0.0f, 2.0f, 0.0f));
        if (getPosition().y < 0) {
            killLight();
            kill();
        }
        else {
            std::set<std::string> tags = getCollisionGroup();
            if (tags.find("obstacle") != tags.end()) {
                killLight();
                kill();
                return;
            }
        }
    }

    glm::vec3 getLight() {
        if (light != 0) {
            return light->getLight();
        }
    }

    void killLight() {
        std::vector<PointLight*>::iterator k = pointLights.begin();
        while (k != pointLights.end()) {
            if (*k == light) {
                *k = 0;
            }
            else {
                k++;
            }
        }
        light->kill();
        //std::cout << pointLights.size() << std::endl;
    }

};
