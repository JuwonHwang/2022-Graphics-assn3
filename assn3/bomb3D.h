#pragma once

#include "sprite3d.h"

class Bomb3D : public Sprite3D {
private:

public:
    Bomb3D(std::string _name, Color _color, Position _position, std::vector<std::vector<Sprite3D*>*> _groups, Position _vel)
        :Sprite3D(_name, _color, _position, _groups, "resource/ball.obj") {
        setVelocity(_vel);
        setAccel(glm::vec3(0.0f, -0.03f, 0.0f));
        setCollisionTag("bomb");
    }

    virtual void update() {
        Sprite3D::update();
        if (getPosition().y < 0) {
            kill();
        }
        else {
            std::set<std::string> tags = getCollisionGroup();
            if (tags.find("tank") != tags.end()) {
                this->kill();
                return;
            }
        }
    }

};
