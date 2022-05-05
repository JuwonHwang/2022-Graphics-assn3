#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/vec3.hpp>
#include <vector>
#include "tank.h"
#include "bomb.h"
#include "land.h"
#include "collision3D.h"
#include "sprite3d.h"
#include "tank3d.h"
#include "ground.h"
#include "camera.h"

bool playing = true;
bool all_pass = false;
bool all_fail = false;

Camera camera;
Sprite3D* cube;
Tank3D* tank;
Tank3D* enemy;
Ground* ground;

void init(void) {
    tank = new Tank3D("tank", blue, Position(0, 0, 10), { &allGroups });
    enemy = new Tank3D("enemy", yellow, Position(0, 0, -10), { &allGroups });
    enemy->setAuto(true);
    enemy->rotate(glm::vec3(0, 180, 0));
    ground = new Ground("ground", grey, Position(0, 0, 0), { &allGroups }, "", { 60,60 });
}

void renderScene(void)
{
    if (!playing) {
        return;
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    /*glEnable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);

    if (hidden_line_removal) {
        glEnable(GL_CULL_FACE);
    }
    else {
        glDisable(GL_CULL_FACE);
    }*/
    

    switch (camera.getMode()) {
    case cmode::THIRD_PERSON:
        camera.View(tank->getPosition(), tank->getRPY(), tank->getRecoil());
        break;
    case cmode::FIRST_PERSON:
        camera.View(tank->getPosition(), tank->getbarrelRPY(), tank->getRecoil());
        break;
    case cmode::TOP_VIEW:
        camera.View(tank->getPosition(), tank->getRPY(), tank->getRecoil());
        break;
    default:
        break;
    }
    glPushMatrix();
    for (size_t i = 0; i < allGroups.size(); i++)
    {
        if (allGroups[i] != NULL) {
            allGroups[i]->draw3d();
        }
        glUseProgram(allGroups[i]->shaderProgram);
        std::cout << "program";
        glBindVertexArray(allGroups[i]->VAO);
        std::cout << "bind vao";
        glDrawArrays(GL_TRIANGLES, 0, 3);
        std::cout << "draw";
        glBindVertexArray(0);
        std::cout << "bind 0";
    }
    glPopMatrix();


    glFlush();
}


void specialkeyboard(int key, int x, int y) {
    if (!playing) {
        return;
    }
    switch (key) {
    case GLUT_KEY_LEFT:
        tank->turn(1);
        break;
    case GLUT_KEY_RIGHT:
        tank->turn(-1);
        break;
    case GLUT_KEY_UP:
        tank->forward(-1);
        break;
    case GLUT_KEY_DOWN:
        tank->forward(1);
        break;
    }
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    if (!playing) {
        return;
    }
    switch (key) {
    case ' ':
        tank->shoot(&allGroups);
        break;
    case 'q':
        tank->powerDown();
        break;
    case 'e':
        tank->powerUp();
        break;
    case 'w':
        tank->rotateBarrel(5);
        break;
    case 's':
        tank->rotateBarrel(-5);
        break;
    case 'a':
        tank->rotateHead(5);
        break;
    case 'd':
        tank->rotateHead(-5);
        break;
    case 'v':
        camera.ChangeMode();
        break;
    case 'r':
        hidden_line_removal = !hidden_line_removal;
        break;
    case 'c':
        all_pass = !all_pass;
        enemy->setWeak(all_pass);
        break;
    case 'f':
        all_fail = !all_fail;
        tank->setWeak(all_fail);
        break;
    }

    glutPostRedisplay();
}

void timer(int value) {
    checkAllCollision(allGroups);
    for (size_t i = 0; i < allGroups.size(); i++)
    {
        if (allGroups[i] != NULL) {
            allGroups[i]->update();
        }
    }
    std::vector<Sprite3D*>::iterator j = allGroups.begin();
    while (j != allGroups.end()) {
        if (*j == NULL) {
            j = allGroups.erase(j);
        }
        else {
            j++;
        }
    }
    glutPostRedisplay();

    if (tank->getHp() <= 0 && enemy->getHp() <= 0) {
        std::cout << "DRAW..." << std::endl;
        playing = false;
    }
    else if (tank->getHp() <= 0) {
        std::cout << "YOU LOSE!" << std::endl;
        playing = false;
    }
    else if (enemy->getHp() <= 0) {
        std::cout << "YOU WIN!!!" << std::endl;
        playing = false;
    }
    else {
        glutTimerFunc(30, timer, 0);
    }
}
    

void main(int argc, char** argv)
{
    srand(time(NULL));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(640, 640);
    init();

    glutCreateWindow("Bored Students - Assn3-1");
    glutDisplayFunc(renderScene);
    glutSpecialFunc(specialkeyboard);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, timer, 0);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glewInit();
    glutMainLoop();
}
