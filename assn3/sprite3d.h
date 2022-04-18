#pragma once
#include "sprite.h"
#include "obj.h"

bool hidden_line_removal = false;

class Sprite3D : public ColoredSprite {
private:
    float roll = 0.0f * PI;
    float pitch = 0.0f * PI;
    float yaw = 0.0f * PI;

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> collider;
    std::vector<std::vector<Sprite3D*>*> groups;
    std::vector<Sprite3D*> subSprite3Ds;

public:
    Sprite3D() {

    }

    Sprite3D(std::string _name, Color _color, Position _position, std::vector<std::vector<Sprite3D*>*> _groups, std::string path): ColoredSprite(_color, _name, _position) {
        
        if(!path.empty())
            loadShape(path);

        groups = _groups;
        //std::cout << getName() << " - group size : " << _groups.size() << std::endl;
        for (size_t i = 0; i < _groups.size(); i++) {
            _groups[i]->push_back(this);
        }

    }

    void kill() {
        for (size_t i = 0; i < groups.size(); i++) {
            for (size_t j = 0; j < groups[i]->size(); j++) {
                if (groups[i]->at(j) == this) {
                    groups[i]->at(j) = NULL;
                }
            }
        }
        for (size_t i = 0; i < subSprite3Ds.size(); i++) {
            subSprite3Ds[i]->kill();
        }
        delete this;
    }

    bool loadShape(std::string path) {
        return loadOBJ(path, vertices, uvs, normals, collider);
    }

    void addSprite3D(Sprite3D* sub) {
        subSprite3Ds.push_back(sub);
    }

    virtual glm::vec3 getRPY() {
        return { roll, yaw, pitch };
    }

    virtual void rotate(glm::vec3 rpy) {
        roll += rpy.x;
        yaw += rpy.y;
        pitch += rpy.z;
    }

    virtual void accelerate(Position vec) {
        setAccel(getAccel() + vec);
    }

    virtual std::vector<glm::vec3> getCollider(const Transform t) {

        Transform transform =  glm::translate(t, getPosition()+getVelocity());
        transform = glm::rotate(transform, yaw * PI / 180, glm::vec3(0.0f, 1.0f, 0.0f));
        transform = glm::rotate(transform, pitch * PI / 180, glm::vec3(0.0f, 0.0f, 1.0f));
        transform = glm::rotate(transform, roll * PI / 180, glm::vec3(1.0f, 0.0f, 0.0f));

        Positions transformMask;
        for (size_t i = 0; i < collider.size(); i++)
        {
            transformMask.push_back(transform * glm::vec4(collider[i], 1));
        }

        for (size_t i = 0; i < subSprite3Ds.size(); i++) {
            Positions subMask = subSprite3Ds[i]->getCollider(transform);
            for (size_t i = 0; i < subMask.size(); i++) {
                transformMask.push_back(subMask[i]);
            }
        }

        return transformMask;
    }

    virtual void draw3d() {
        glPushMatrix();
        glTranslatef(getPosition().x, getPosition().y, getPosition().z);
        glRotatef(roll, 1.0f, 0.0f, 0.0f);
        glRotatef(pitch, 0.0f, 0.0f, 1.0f);
        glRotatef(yaw, 0.0f, 1.0f, 0.0f);

        if (!hidden_line_removal) {
            glColor3f(getColor()[0], getColor()[1], getColor()[2]);
            glPolygonMode(GL_FRONT, GL_FILL);

            glBegin(GL_TRIANGLES);
            for (size_t i = 0; i < vertices.size(); i++) {
                glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
            }
            glEnd();
        }

        glColor3f(getColor()[0] * 0.2, getColor()[1] * 0.2, getColor()[2] * 0.2);
        glPolygonMode(GL_FRONT, GL_LINE);
        glLineWidth(1.5);

        glBegin(GL_TRIANGLES);
        for (size_t i = 0; i < vertices.size(); i++) {
            glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
        }
        glEnd();

        for (size_t i = 0; i < subSprite3Ds.size(); i++) {
            subSprite3Ds[i]->draw3d();
        }
        glPopMatrix();
    }

    void setColorAll(Color c) {
        setColor(c);
        for (size_t i = 0; i < subSprite3Ds.size(); i++) {
            subSprite3Ds[i]->setColorAll(c);
        }
    }

};



std::vector<Sprite3D*> allGroups = {};