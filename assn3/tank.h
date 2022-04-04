#pragma once
#include "spriteGroup.h"
#include "shapes.h"
#include "bomb.h"
#include <string>

class Tank : public SpriteGroup {
private:
    float hp = 3;
    SpriteGroup* upperBody;
    SpriteGroup* lowerBody;
    SpriteGroup* wheels;
    SpriteGroup* gunBarrel;
    int maxBombs = 1;
    std::vector<Sprite*> bombs;
    bool weak = false;

    int shootTimer = 0;
    float dirMax = 0;
    float dirMin = 0;

public:
    int power = 3;
    
    Tank(std::vector<std::vector<Sprite*>*> _groups, 
        std::string _name, 
        Position _position,
        Color _color ) : SpriteGroup(_groups, _name, _position) { // 탱크 생성자
        // 모든 멤버를 하위 그룹에 추가
        upperBody = new SpriteGroup();
        lowerBody = new SpriteGroup();
        wheels = new SpriteGroup();
        gunBarrel = new SpriteGroup();

        const std::string tag = "tank";

        Color white = _color;
        Color frame_color = Color(0.5f, 0.5f, 0.5f);

        PolygonSprite* upperSp = new PolygonSprite(
            "upperBody",
            white, // color
            glm::vec3(0.0f, 0.1f, 0.0f), // position
            Shape::Circle(0.05f, 0.0f, 0.0f), // Shape
            Shape::Circle(0.05f, 0.0f, 0.0f)); // Mask
        upperSp->setCollisionTag(tag);
        upperBody->addSprite(upperSp);

        PolygonSprite* lowerSp = new PolygonSprite(
            "lowerBody",
            white, // color
            glm::vec3(0.0f, 0.0f, 0.0f), // position
            Shape::Rectangle(0.2f, 0.1f, 0.1f, 0.0f), // Shape
            Shape::Rectangle(0.2f, 0.1f, 0.1f, 0.0f)); // Mask
        lowerSp->setCollisionTag(tag);
        lowerBody->addSprite(lowerSp);

        for (int i = 0; i < 6; i++) {
            SpriteGroup* wheel_unit = new SpriteGroup();

            PolygonSprite* wheel = new PolygonSprite(
                "wheel"+std::to_string(i),
                white, // color
                glm::vec3(0.0f,0.0f,0.0f), // position
                Shape::Circle(0.015f, 0, 0, 0.5f),
                Shape::Circle(0.015f, 0, 0, 0.5f)); // Shape
            PolygonSprite* wheel_frame = new PolygonSprite(
                "wheel_frame" + std::to_string(i),
                frame_color, // color
                glm::vec3(0.0f,0.0f,0.0f), // position
                Shape::Rectangle(0.03f, 0.005f, 0.015f, 0.0015f),
                Shape::Rectangle(0.03f, 0.005f, 0.015f, 0.0015f)); // Shape
            wheel_unit->addSprite(wheel);
            wheel_unit->addSprite(wheel_frame);
            wheel_unit->setPosition(glm::vec3((i - 2.5f) * 0.03f, -0.01f, 0.0f));
            wheels->addSubGroup(wheel_unit);
        }


        PolygonSprite* gunBarrelSp = new PolygonSprite(
            "gunBarrel",
            white, // color
            glm::vec3(0.0f, 0.0f, 0.0f), // position
            Shape::Rectangle(0.2f, 0.01f, 0.0f, 0.005f), // Shape
            Positions()); // Mask
        gunBarrelSp->rotate(0.1f * PI);
        gunBarrel->addSprite(gunBarrelSp);
        gunBarrel->setPosition(Position(0.0f, 0.1f, 0.0f));

        addSubGroup(gunBarrel);
        addSubGroup(upperBody);
        addSubGroup(lowerBody);
        addSubGroup(wheels);

        setCollisionTag("tank");
    }

    void rotateGunBarrel(const float dir) {
        gunBarrel->getSprites()[0]->rotate(dir);
        float d = gunBarrel->getSprites()[0]->getRotation();
        d = std::min(d, dirMax);
        d = std::max(d, dirMin);
        gunBarrel->getSprites()[0]->setRotation(d);
    }

    const Position getBarrelFrontPos() {
        Transform transform = Transform(1.0f);
        transform = glm::translate(transform, getPosition());
        transform = glm::translate(transform, gunBarrel->getPosition());
        transform = glm::translate(transform, gunBarrel->getSprites()[0]->getPosition());
        transform = glm::rotate(transform, gunBarrel->getSprites()[0]->getRotation(), glm::vec3(0.0f, 0.0f, 1.0f));

        glm::vec4 drawPosition = transform * glm::vec4(0.2f, 0.005f, 0.0f, 1);
        return drawPosition;
    }

    void shoot(std::vector<std::vector<Sprite*>*> allGroup) {
        if (hp > 0 && bombs.size() < maxBombs) {
            allGroup.push_back(&bombs);
            Bomb* bomb = new Bomb(allGroup, "bomb", getBarrelFrontPos()); // 폭탄 생성
            const float dir = gunBarrel->getSprites()[0]->getRotation();
            glm::vec3 _vel = glm::vec3(cosf(dir), sinf(dir), 0) * 0.01f * float(power);
            bomb->setVelocity(_vel);
        }
    }

    virtual void update() {
        if (checkCollision()) {
            setVelocity(Position(0, 0.0f, 0.0f));
        }
        if (getVelocity().x > 0) {
            for (int i = 0; i < 6; i++) {
                wheels->getSubGroup()[i]->getSprites()[0]->rotate(-0.1);
                wheels->getSubGroup()[i]->getSprites()[1]->rotate(-0.1);
            }
        }
        else if(getVelocity().x < 0){
            for (int i = 0; i < 6; i++) {
                wheels->getSubGroup()[i]->getSprites()[0]->rotate(0.1);
                wheels->getSubGroup()[i]->getSprites()[1]->rotate(0.1);
            }
        }
        SpriteGroup::update();
        setVelocity(Position(0, 0, 0));
        if (hp <= 0) {
            this->kill();
        }
    }

    float getHp() {
        return hp;
    }

    virtual void kill() {
        while (bombs.size() > 0) {
            bombs[0]->kill();
        }
        SpriteGroup::kill();
    }

    bool checkCollision(void) {
        //bool left = (getPosition()[0] - 0.1f + speed) <= -1.0f;
        //bool right = (getBarrelFrontPos()[0] + speed) >= 1.0f;
        glm::vec4 rect = getRectangle(Position(0.0f, 0.0f, 0.0f), 0.0f);
        //std::cout << rect[0] << ", " << rect[1] << std::endl;
        bool left = (rect[0] + getVelocity().x) <= -1.0f;
        bool right = (rect[1] + getVelocity().x) >= 1.0f;

        std::set<std::string> tags = getCollisionGroup();
        if (tags.find("tank") != tags.end()) {
            return true;
        }
        if (tags.find("bomb") != tags.end()) {
            if (weak) hp = 0;
            hp -= 1;
        }
        return ((getVelocity().x < 0) && left || (getVelocity().x > 0) && right);
    }

    void setDirMinMax(float min, float max) {
        dirMin = min;
        dirMax = max;
        rotateGunBarrel(0);
    }

    int getShootTimer(void) {
        return shootTimer;
    }

    void setShootTimer(int _timer) {
        shootTimer = _timer;
    }

    bool randShoot() {
        if (hp <= 0) {
            return false;
        }
        shootTimer += 1;
        if (rand() % 20 < 2) {
            angleUpGunBarrel();
        }
        if (rand() % 20 < 2) {
            angleDownGunBarrel();
        }
        if (rand() % 20 < 2) {
            PowerUp();
        }
        if (rand() % 20 < 2) {
            PowerDown();
        }
        if (shootTimer > 30) {
            shootTimer = 0;
            return true;
        }
        else {
            return false;
        }
    }

    void angleUpGunBarrel() {
        //std::cout << gunBarrel->getSprites()[0]->getRotation();
        rotateGunBarrel(0.1);
    }

    void angleDownGunBarrel() {
        //std::cout << gunBarrel->getSprites()[0]->getRotation();
        
        rotateGunBarrel(-0.1);
        
    }

    void changeGunBarrelColor(Color gun_color) {
        this->gunBarrel->setColor(gun_color);
    }

    void updateGunBarrelColor() {

        switch (power) {
        case 1:
            changeGunBarrelColor(blue);
            break;
        case 2:
            changeGunBarrelColor(purple);
            break;
        case 3:
            changeGunBarrelColor(green);
            break;
        case 4:
            changeGunBarrelColor(yellow);
            break;
        case 5:
            changeGunBarrelColor(red);
            break;
        }
    }

    void PowerUp() {
        if (power < 5) {
            power += 1;
        }
        updateGunBarrelColor();
    }

    void PowerDown() {
        if (power > 1) {
            power -= 1;
        }
        updateGunBarrelColor();
    }

    void setWeak(bool _weak) {
        weak = _weak;
    }
};
