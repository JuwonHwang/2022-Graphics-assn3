#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/vec3.hpp>


#include "textReader.h"
#include "shader.h"

#include <vector>

#include "tank.h"
#include "bomb.h"
#include "land.h"
#include "collision3D.h"
#include "sprite3d.h"
#include "tank3d.h"
#include "ground.h"
#include "camera.h"
#include "light.h"
#include "util.h"
#include "light.h"

unsigned int VAO;

bool playing = true;
bool all_pass = false;
bool all_fail = false;
bool diff_tex_mapping = true;
bool normal_mapping = true;

Camera camera;
Sprite3D* cube = 0;
Tank3D* tank = 0;
Tank3D* enemy = 0;
Ground* ground = 0;
Sun* sun = 0;

void init(void) {
    tank = new Tank3D("tank", blue, Position(0, 0, 15), { &allGroups });
    enemy = new Tank3D("enemy", yellow, Position(0, 0, -15), { &allGroups });
    enemy->setAuto(true);
    enemy->rotate(glm::vec3(0, 180, 0));
    ground = new Ground("ground", grey, Position(0, 0, 0), { &allGroups }, "", { 20,20 });
    sun = new Sun();
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
    case 'x': // Gouraud shading & Phong shading
        gouraud = !gouraud;
        break;
    case 't': // Diffuse texture mapping
        diff_tex_mapping = !diff_tex_mapping;
        break;
    case 'n': // Normal mapping
        normal_mapping = !normal_mapping;
        break;
    }

    glutPostRedisplay();
}

void timer(int value) {
    checkAllCollision(allGroups);
    sun->update();
    dirLight = sun->getLight();
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

    std::vector<PointLight*>::iterator k = pointLights.begin();
    while (k != pointLights.end()) {
        if (*k == NULL) {
            k = pointLights.erase(k);
        }
        else {
            k++;
        }
    }

    glutPostRedisplay();

    if (tank && tank->getHp() <= 0 && enemy && enemy->getHp() <= 0) {
        std::cout << "DRAW..." << std::endl;
        playing = false;
    }
    else if (tank && tank->getHp() <= 0) {
        std::cout << "YOU LOSE!" << std::endl;
        playing = false;
    }
    else if (enemy && enemy->getHp() <= 0) {
        std::cout << "YOU WIN!!!" << std::endl;
        playing = false;
    }
    else {
        glutTimerFunc(30, timer, 0);
    }
}

//Rendering
void display();				//기본 랜더링 코드


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(600, 600);
	glutCreateWindow("OpenGL");
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutDisplayFunc(display);
	glutIdleFunc(display);
    glutSpecialFunc(specialkeyboard);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, timer, 0);


    glewInit();
	setShaders();

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    PLoc = glGetUniformLocation(program_shader, "P");
    vertexColorLocation = glGetUniformLocation(program_shader, "ourColor");
    MVLoc = glGetUniformLocation(program_shader, "MV");
    lightPos = glGetUniformLocation(program_shader, "LP");
    ap = glGetUniformLocation(program_shader, "AP");
    dp = glGetUniformLocation(program_shader, "DP");
    sp = glGetUniformLocation(program_shader, "SP");
    shn = glGetUniformLocation(program_shader, "Shininess");
    gr = glGetUniformLocation(program_shader, "is_gouraud");
    pointLightNumLoc = glGetUniformLocation(program_shader, "pointLightNum");
    pointLightsLoc[0] = glGetUniformLocation(program_shader, "pointLights[0]");
    pointLightsLoc[1] = glGetUniformLocation(program_shader, "pointLights[1]");
    pointLightsLoc[2] = glGetUniformLocation(program_shader, "pointLights[2]");
    pointLightsLoc[3] = glGetUniformLocation(program_shader, "pointLights[3]");
    pointLightsLoc[4] = glGetUniformLocation(program_shader, "pointLights[4]");
    pointLightsLoc[5] = glGetUniformLocation(program_shader, "pointLights[5]");
    pointLightsLoc[6] = glGetUniformLocation(program_shader, "pointLights[6]");
    pointLightsLoc[7] = glGetUniformLocation(program_shader, "pointLights[7]");
    pointLightsLoc[8] = glGetUniformLocation(program_shader, "pointLights[8]");
    pointLightsLoc[9] = glGetUniformLocation(program_shader, "pointLights[9]");


    init();

	glutMainLoop();
	return 0;
}

void display()
{
    if (!playing) {
        return;
    }

	//Clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);

    // ..:: 드로잉 코드 (렌더링 루프 내부) :: ..
    // 4. 오브젝트를 그립니다.
    glUseProgram(program_shader);
    glBindVertexArray(VAO);

    if (hidden_line_removal) {
        glEnable(GL_CULL_FACE);
    }
    else {
        glDisable(GL_CULL_FACE);
    }

    //Draw

    switch (camera.getMode()) {
    case cmode::THIRD_PERSON:
        projection_view = camera.View(tank->getPosition(), tank->getRPY(), tank->getRecoil());
        break;
    case cmode::FIRST_PERSON:
        projection_view = camera.View(tank->getPosition(), tank->getbarrelRPY(), tank->getRecoil());
        break;
    case cmode::TOP_VIEW:
        projection_view = camera.View(tank->getPosition(), tank->getRPY(), tank->getRecoil());
        break;
    default:
        break;
    }

    glUniformMatrix4fv(PLoc, 1, GL_FALSE, glm::value_ptr(projection_view));
    glUniform4f(lightPos, dirLight.x, dirLight.y, dirLight.z, 0.0f);


    glUniform1i(pointLightNumLoc, (GLint)pointLights.size());
    //std::cout << pointLightNumLoc << " ,, " << pointLights.size() << std::endl;
    for (size_t i = 0; i < std::min(pointLights.size(),(size_t)10); i++) {
        glm::vec3 light = pointLights[i]->getLight();
        glUniform4f(pointLightsLoc[i], light.x, light.y, light.z, 1.0f);
    }

    glUniform4f(ap, 0.3f, 0.3f, 0.3f, 1.0f);
    glUniform4f(dp, 0.2f, 0.2f, 0.2f, 1.0f);
    glUniform4f(sp, 0.3f, 0.3f, 0.3f, 1.0f);
    glUniform1f(shn, 1);
    if (gouraud) {
        glUniform1f(gr, true);
    }
    else {
        glUniform1f(gr, false);
    }

    model_view_mat.push(glm::mat4(1.0f));
    for (size_t i = 0; i < allGroups.size(); i++)
    {
        if (allGroups[i] != NULL) {
            allGroups[i]->draw3d();
        }
    }
    while (!model_view_mat.empty())model_view_mat.pop();


	glFlush();
	glutSwapBuffers();
}