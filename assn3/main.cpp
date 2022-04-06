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
std::vector<Sprite3D*> allGroups = {};
bool all_pass = false;
bool all_fail = false;

Camera camera;
Sprite3D* cube;
Tank3D* tank;
Ground* ground;

void init(void) {
    //cube = new Sprite3D("Cube", grey, Position(0, 0, 0), { &allGroups }, "resource/body.obj");
    tank = new Tank3D("tank", grey, Position(0, 0, 0), { &allGroups }, "");
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
        camera.View(tank->getPosition(), tank->getRPY());
        break;
    case FIRST_PERSON:
        camera.View(tank->getPosition(), tank->getbarrelRPY());
        break;
    case TOP_VIEW:

        break;
    default:
        break;
    }
    glPushMatrix();
    for (size_t i = 0; i < allGroups.size(); i++)
    {
        allGroups[i]->draw3d();
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
        tank->rotate(Position(0.0f, 5, 0.0f));
        break;
    case GLUT_KEY_RIGHT:
        tank->rotate(Position(0.0f, -5, 0.0f));
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
        break;
    case 'q':
        break;
    case 'e':
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
    }
    glutPostRedisplay();
}

void timer(int value) {
    std::vector<Sprite*> new_all;
    new_all.insert(new_all.end(), allGroups.begin(), allGroups.end());
    for (size_t i = 0; i < new_all.size(); i++)
    {
        new_all[i]->update();
    }
    glutPostRedisplay();
    glutTimerFunc(30, timer, 0);
}

void main(int argc, char** argv)
{
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
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glewInit();
    glutMainLoop();
}
