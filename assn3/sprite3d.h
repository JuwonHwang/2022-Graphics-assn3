#pragma once
#include <stack>
#include "sprite.h"
#include "obj.h"
#include "shader.h"
#include <glm/vec3.hpp>

glm::mat4 projection_view = glm::mat4(1.0f);
std::stack<glm::mat4> model_view_mat;

int vertexColorLocation;
int MVLoc;
int lightPos;
int ap, dp, sp, shn, gr;

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

    unsigned int VBO;
    glm::mat4 mv;

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

        glGenBuffers(1, &VBO);

    }

    virtual void kill() {
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
        glDeleteBuffers(1, &VBO);
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

        glm::mat4 t = model_view_mat.top();
        mv = glm::translate(t, getPosition());
        mv = glm::rotate(mv, yaw * PI / 180, glm::vec3(0.0f, 1.0f, 0.0f));
        mv = glm::rotate(mv, pitch * PI / 180, glm::vec3(0.0f, 0.0f, 1.0f));
        mv = glm::rotate(mv, roll * PI / 180, glm::vec3(1.0f, 0.0f, 0.0f));
        model_view_mat.push(mv);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0][0], GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glUniformMatrix4fv(MVLoc, 1, GL_FALSE, glm::value_ptr(mv));
        glUniform4f(lightPos, 0.0f, 1.0f, 1.0f, 0.0f);
        glUniform4f(ap, 0.5f, 0.5f, 0.5f, 0.5f);
        glUniform4f(dp, 0.5f, 0.5f, 0.5f, 1.0f);
        glUniform4f(sp, 0.5f, 0.5f, 0.5f, 1.0f);
        glUniform1f(shn, 1);
        if (gouraud) {
            glUniform1f(gr, true);
        }
        else {
            glUniform1f(gr, false);
        }

        

        if (hidden_line_removal) {
            /*glLineWidth(1.5f);
            glUniform4f(vertexColorLocation, getColor()[0], getColor()[1], getColor()[2], 1.0f);
            for (size_t i = 0; i < vertices.size(); i++) {
                glDrawArrays(GL_LINES, i, 3);
            }*/

            glUniform4f(vertexColorLocation, getColor()[0], getColor()[1], getColor()[2], 1.0f);
            for (size_t i = 0; i < vertices.size(); i++) {
                glDrawArrays(GL_TRIANGLES, i, 3);
            }
        }
        else {
            glLineWidth(1.0f);
            glUniform4f(vertexColorLocation, getColor()[0], getColor()[1], getColor()[2], 1.0f);
            for (size_t i = 0; i < vertices.size(); i++) {
                glDrawArrays(GL_LINES, i, 3);
            }
        }
        

        for (size_t i = 0; i < subSprite3Ds.size(); i++) {
            subSprite3Ds[i]->draw3d();
        }

        if(!model_view_mat.empty())model_view_mat.pop();

    }

    void setColorAll(Color c) {
        setColor(c);
        for (size_t i = 0; i < subSprite3Ds.size(); i++) {
            subSprite3Ds[i]->setColorAll(c);
        }
    }

};



std::vector<Sprite3D*> allGroups = {};