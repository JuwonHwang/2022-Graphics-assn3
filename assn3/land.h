#pragma once
#include "spriteGroup.h"
#include "shapes.h"
#include <string>

class Land : public SpriteGroup {
private:

public:
    Land(std::vector<std::vector<Sprite*>*> _groups, std::string _name, Position _position) : SpriteGroup(_groups, _name, _position) { // 땅 생성자

        Color white = Color(0.8f, 1.0f, 1.0f);

        PolygonSprite* land = new PolygonSprite(
            "land_sp",
            white, // color
            glm::vec3(0.0f, -0.03f, 0.0f), // position
            Shape::Rectangle(2.0f, 0.01f, 1.0f, 0.0f), // Shape
            Shape::Rectangle(2.0f, 0.01f, 1.0f, 0.0f)); // Mask
        this->addSprite(land); // SpriteGroup에 Sprite 추가

    }
};