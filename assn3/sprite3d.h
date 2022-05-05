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

    unsigned int VBO;
    //unsigned int VAO;
    unsigned int vertexShader;
    unsigned int fragmentShader;
    //unsigned int shaderProgram;

    const char* vs = "#version 330 core layout (location=0) in vec3 aPosition; void main(){ gl_Position=vec4(aPosition.x,aPosition.y,aPosition.z,1.0);}";
    const char* fs = "#version 330 core out vec4 FragColor; void main(){ FragColor=vec4(1.0f,0.5f,0.2f,1.0f);}";

public:
    unsigned int VAO;
    unsigned int shaderProgram;

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

        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vs, NULL);
        glCompileShader(vertexShader);
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fs, NULL);
        glCompileShader(fragmentShader);

        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        /*glTranslatef(getPosition().x, getPosition().y, getPosition().z);
        glRotatef(roll, 1.0f, 0.0f, 0.0f);
        glRotatef(pitch, 0.0f, 0.0f, 1.0f);
        glRotatef(yaw, 0.0f, 1.0f, 0.0f);

        if (hidden_line_removal) {
            glColor3f(1.0f, 1.0f, 1.0f);
            glPolygonMode(GL_FRONT, GL_FILL);
            
            glBegin(GL_TRIANGLES);
            for (size_t i = 0; i < vertices.size(); i++) {
                glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
            }
            glEnd();
        }

        glColor3f(getColor()[0], getColor()[1], getColor()[2]);
        if (hidden_line_removal) {
            glPolygonMode(GL_FRONT, GL_LINE);
        }
        else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        glLineWidth(1.5);

        glBegin(GL_TRIANGLES);
        for (size_t i = 0; i < vertices.size(); i++) {
            glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
        }
        glEnd();*/

        glGenVertexArrays(1,&VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        //std::cout << sizeof(vertices);
        float datas[3*sizeof(vertices)];
        for (size_t i = 0; i < sizeof(vertices); i++) {
            datas[3*i] = float(vertices[i].x);
            datas[3*i+1] = float(vertices[i].y);
            datas[3*i+2] = float(vertices[i].s);
            std::cout << "datas problem";
        }

        
        glBufferData(GL_ARRAY_BUFFER, sizeof(datas), datas, GL_STATIC_DRAW);
        std::cout << "bufferdata";
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        std::cout << "attrib";
        glEnableVertexAttribArray(0);
        std::cout << "enable";




        for (size_t i = 0; i < subSprite3Ds.size(); i++) {
            subSprite3Ds[i]->draw3d();
            std::cout << "sub";
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