#include <iostream>
#include <GL/freeglut.h>
#include "cloth.h"
#include "sphere.h"

float k = 2000;
float deltat = 0.008f;
double small_t = 1e-6;

Sphere sphere1(vec3(5,-5,2), 3.0f);
Sphere sphere2(vec3(5,-5,5), 3.0f);
Sphere sphere3(vec3(8,-5,5), 3.0f);
Sphere sphere4(vec3(8,-5.1,5), 3.0f);
Sphere sphere5(vec3(5,-2.9,5), 3.0f);

Cloth *currCloth = nullptr;
Cloth cloth1(10, 10, 1.0f, k/8, nullptr, 1);
Cloth cloth2(50, 25, 0.4f, k, nullptr, 2);
Cloth cloth3(0.2f, k, vec3(0,0,0), vec3(10.0f,0,0), vec3(0,-10.0f,0), nullptr, 2, true);
Cloth cloth4(0.1f, k, vec3(4,0,0), vec3(7.92f,-2.94f,0), vec3(1.06,-3.92f,0), nullptr, 2, true);
Cloth cloth5(0.2f, k, vec3(0,0,0), vec3(10,0,0), vec3(0,0,10), &sphere1, 4, true);
Cloth cloth6(0.2f, k, vec3(0,0,0), vec3(0,0,10), vec3(10,0,0), &sphere2, 4, true);
Cloth cloth7(0.2f, k, vec3(0,0,0), vec3(0,0,10), vec3(10,0,0), &sphere3, 4, true);
Cloth cloth8(0.2f, k, vec3(0,-2,0), vec3(0,-2,10), vec3(10,-2,0), &sphere4, 4, false);
Cloth cloth9(0.2f, k, vec3(0,0,0), vec3(0,0,10), vec3(10,0,0), &sphere5, 4, false);

void update() {
    Cloth &cloth = *currCloth;
    for(int i = 0; i < cloth.steps; i++) {
        cloth.computeForces();
        cloth.integrate(deltat);
    }
    glutPostRedisplay();
}

void display() {
    Cloth &cloth = *currCloth;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(cloth.sphere) {
        Sphere *currSphere = cloth.sphere;
        
        glColor3f(0,1,0);
        glBegin(GL_TRIANGLES);
        for(unsigned i = 0; i < currSphere->triangleIndices.size(); i += 3) {
            auto &p1 = currSphere->particles[currSphere->triangleIndices[i]];
            auto &p2 = currSphere->particles[currSphere->triangleIndices[i+1]];
            auto &p3 = currSphere->particles[currSphere->triangleIndices[i+2]];

            glVertex3f(p1[0],p1[1],p1[2]);
            glVertex3f(p2[0],p2[1],p2[2]);
            glVertex3f(p3[0],p3[1],p3[2]);
        }
        glEnd();

        glColor3f(1,1,1);
        glBegin(GL_LINES);
        for(unsigned i = 0; i < currSphere->lineIndices.size(); i += 2) {
            auto &p1 = currSphere->particles[currSphere->lineIndices[i]];
            auto &p2 = currSphere->particles[currSphere->lineIndices[i+1]];

            glVertex3f(p1[0],p1[1],p1[2]+small_t);
            glVertex3f(p2[0],p2[1],p2[2]+small_t);
        }
        glEnd();
    }

    glColor3f(0,0,1);
    glBegin(GL_TRIANGLES);
    for(int y = 0; y < cloth.height-1; y++) {
        for(int x = 0; x < cloth.width-1; x++) {
            auto &p1 = cloth.particles[cloth.convertIndex(x, y)];
            auto &p2 = cloth.particles[cloth.convertIndex(x+1, y)];
            auto &p3 = cloth.particles[cloth.convertIndex(x, y+1)];
            auto &p4 = cloth.particles[cloth.convertIndex(x+1, y+1)];

            glVertex3f(p1.position[0],p1.position[1],p1.position[2]);
            glVertex3f(p2.position[0],p2.position[1],p2.position[2]);
            glVertex3f(p3.position[0],p3.position[1],p3.position[2]);

            glVertex3f(p2.position[0],p2.position[1],p2.position[2]);
            glVertex3f(p3.position[0],p3.position[1],p3.position[2]);
            glVertex3f(p4.position[0],p4.position[1],p4.position[2]);
        }
    }
    glEnd();

    glColor3f(1,1,1);
    glBegin(GL_LINES);
    for(auto &s : cloth.springs) {
        auto &p1 = cloth.particles[s.particle1];
        auto &p2 = cloth.particles[s.particle2];

        glVertex3f(p1.position[0],p1.position[1],p1.position[2]+small_t);
        glVertex3f(p2.position[0],p2.position[1],p2.position[2]+small_t);
    }
    glEnd();
    
    glColor3f(1,0,0);
    glPointSize(3);
    glBegin(GL_POINTS);
    for(auto &p : cloth.particles) {
        glVertex3f(p.position[0],p.position[1],p.position[2]+small_t+small_t+small_t);
    }
    glEnd();

    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    if(key == 'r') currCloth->reset();
    if(key == '1') currCloth = &cloth1;
    if(key == '2') currCloth = &cloth2;
    if(key == '3') currCloth = &cloth3;
    if(key == '4') currCloth = &cloth4;
    if(key == '5') currCloth = &cloth5;
    if(key == '6') currCloth = &cloth6;
    if(key == '7') currCloth = &cloth7;
    if(key == '8') currCloth = &cloth8;
    if(key == '9') currCloth = &cloth9;
    if(key == 'w') currCloth->wind = vec3(0, 0, -3);
    else if(key == 's') currCloth->wind = vec3(0, 0, 3);
    else if(key == 'a') currCloth->wind = vec3(-3, 0, 0);
    else if(key == 'd') currCloth->wind = vec3(3, 0, 0);
    else currCloth->wind = vec3();
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    currCloth = &cloth1;

    glutInit(&argc, argv);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(1906, 974);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutCreateWindow("Cloth Simulation");
    glutDisplayFunc(display);
    glutIdleFunc(update);
    glutKeyboardFunc(keyboard);

    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-2, 29.31, -15, 1, -20, 20);
    // glOrtho(-1, 19, -19, 1, -10, 10);

    glutMainLoop();

    return 0;
}