#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/vec3.hpp>
#include <vector>
#include "tank.h"
#include "bomb.h"
#include "land.h"
#include "collision.h"
#include "sprite3d.h"
#include "tank3d.h"
#include "ground.h"
#include "camera.h"

bool playing = true;
bool all_pass = false;
bool all_fail = false;
bool hidden = false;

Camera camera;
Sprite3D* cube;
Tank3D* tank;
Tank3D* enemy;
Ground* ground;

void init(void) {
    //cube = new Sprite3D("Cube", grey, Position(0, 0, 0), { &allGroups }, "resource/body.obj");
    tank = new Tank3D("tank", grey, Position(0, 0, 20), { &allGroups });
    enemy = new Tank3D("enemy", red, Position(0, 0, -20), { &allGroups });
    enemy->setAuto(true);
    enemy->rotate(glm::vec3(0, 180, 0));
    ground = new Ground("ground", grey, Position(0, 0, 0), { &allGroups }, "", { 40,40 });
}

void renderScene(void)
{
    if (!playing) {
        return;
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    switch (camera.getMode()) {
    case THIRD_PERSON:
        camera.View(tank->getPosition(), tank->getRPY(), tank->getRecoil());
        break;
    case FIRST_PERSON:
        camera.View(tank->getPosition(), tank->getbarrelRPY(), tank->getRecoil());
        break;
    case TOP_VIEW:
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
    case 'f':
        break;
    case 'v':
        camera.ChangeMode();
        break;
    case 'r':
        if (hidden == false) {
            hidden = true;
        }
        else {
            hidden = false;
        }
        break;
    }

    glutPostRedisplay();
}

void timer(int value) {
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
    glutTimerFunc(30, timer, 0);
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
