#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/vec3.hpp>
#include <vector>
#include "colors.h"

#define PI (3.14159265359f)
#define PI2 (6.28318530718f)

typedef glm::vec3 Position;
typedef std::vector<glm::vec3> Positions;

namespace Shape
{
    /*
    너비, 높이, 중심점(x, y)를 받아 직사각형 vertices를 반환함
    */ 
    Positions Rectangle(const float width, const float height, const float x, const float y) {
        const glm::vec3 center = glm::vec3(x, y, 0.0f);
        return {
            glm::vec3(0.0f, 0.0f, 0.0f) - center,
            glm::vec3(0.0f, height, 0.0f) - center,
            glm::vec3(width, height, 0.0f) - center,
            glm::vec3(width, 0.0f, 0.0f) - center,
        };
    }

    /*
    반지름, 중심점(x, y)를 받아 원 vertices 반환함.
    원을 그리는 정확도(점의 개수)를 정할 수 있음.
    */
    Positions Circle(const float radius, const float x, const float y, const float accuracy = 1.0f) {
        const glm::vec3 center = glm::vec3(x, y, 0.0f);
        Positions position = {};
        int count = int(24 * accuracy);
        for (int i = 0; i < count; i++) {
            position.push_back(glm::vec3(radius * cosf(PI2 * i / count), radius * sinf(PI2 * i / count), 0.0f) + center);
        }
        return position;
    }

    /*
    너비, 높이, 중심점(x, y)를 받아 타원 vertices 반환함.
    타원을 그리는 정확도(점의 개수)를 정할 수 있음.
    */
    Positions Ellipse(const float width, const float height, const float x, const float y, const float accuracy = 1.0f) {
        const glm::vec3 center = glm::vec3(x, y, 0.0f);
        Positions position = {};
        int count = int(24 * accuracy);
        for (int i = 0; i < count; i++) {
            position.push_back(glm::vec3(width * cosf(PI2 * i / count), height * sinf(PI2 * i / count), 0.0f) + center);
        }
        return position;
    }
}
