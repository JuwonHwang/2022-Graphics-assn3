#pragma once
#include "sprite.h"
#include "obj.h"
#include "collider.h"


float colorScale = 1.0f;

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

    virtual void draw3d() {
        glPushMatrix();
        glTranslatef(getPosition().x, getPosition().y, getPosition().z);
        glRotatef(roll, 1.0f, 0.0f, 0.0f);
        glRotatef(pitch, 0.0f, 0.0f, 1.0f);
        glRotatef(yaw, 0.0f, 1.0f, 0.0f);
        
        glColor3f(getColor()[0], getColor()[1], getColor()[2]);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glBegin(GL_TRIANGLES);
        for (size_t i = 0; i < vertices.size(); i++) {
            glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
        }
        glEnd();

        glColor3f(0, 0, 0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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

    virtual Collider getCollider(const Transform t, const Transform r) { 

        Transform transform = glm::translate(t, getPosition());
        transform = glm::rotate(transform, roll, glm::vec3(1.0f, 0.0f, 0.0f));
        transform = glm::rotate(transform, pitch, glm::vec3(0.0f, 0.0f, 1.0f));
        transform = glm::rotate(transform, yaw, glm::vec3(0.0f, 1.0f, 0.0f));

        Transform t_r = glm::rotate(r, roll, glm::vec3(1.0f, 0.0f, 0.0f));
        t_r = glm::rotate(t_r, pitch, glm::vec3(0.0f, 0.0f, 1.0f));
        t_r = glm::rotate(t_r, yaw, glm::vec3(0.0f, 1.0f, 0.0f));

        Position maxbox(0,0,0);
        for (size_t i = 1; i < collider.size(); i++)
        {
            maxbox.x = glm::max(maxbox.x, glm::abs(collider[i].x));
            maxbox.y = glm::max(maxbox.y, glm::abs(collider[i].y));
            maxbox.z = glm::max(maxbox.z, glm::abs(collider[i].z));
        }

        Positions rotNormal;
        for (size_t i = 1; i < 3; i++)
        {
            rotNormal.push_back(t_r * glm::vec4(normals[i], 1));
        }

        return Collider(transform * glm::vec4(0, 0, 0, 1), maxbox, rotNormal);
    }

};



std::vector<Sprite3D*> allGroups = {};