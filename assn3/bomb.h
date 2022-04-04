#pragma once
#include "spriteGroup.h"
#include "shapes.h"
#include <string>

class Bomb : public SpriteGroup {
private:

public:
    Bomb(std::vector<std::vector<Sprite*>*> _groups, std::string _name, Position _position) : SpriteGroup(_groups, _name, _position) { // ÆøÅº »ý¼ºÀÚ

        Color white = Color(1.0f, 1.0f, 1.0f);

        PolygonSprite* ball = new PolygonSprite(
            "ball",
            white, // color
            glm::vec3(0.0f, 0.0f, 0.0f), // position
            Shape::Circle(0.02f, 0.0f, 0.0f), // Shape
            Shape::Circle(0.02f, 0.0f, 0.0f)); // Mask
        this->addSprite(ball); // SpriteGroup¿¡ Sprite Ãß°¡
        setCollisionTag("bomb");
        setAccel(glm::vec3(0.0f, -0.001f, 0.0f));
    }

    bool landedChecker() { // ÆøÅºÀÌ ¶¥¿¡ ´ê¾Ò´ÂÁö È®ÀÎÇÏ´Â ÇÔ¼ö
        if (this->getPosition()[1] <= 0.0f) {
            return true;
        }
        else {
            return false;
        }
    }

    virtual void update() {
        SpriteGroup::update();
        std::set<std::string> tags = getCollisionGroup();
        if (tags.find("tank") != tags.end()) {
            this->kill();
            return;
        }
        else if (landedChecker()) {
            this->kill();
            return;
        }
    }
};
