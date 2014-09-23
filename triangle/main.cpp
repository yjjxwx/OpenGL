#include <iostream>

using namespace std;
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

const GLint screenWidth = 512;
const GLint screenHeight = 512;
void init(){
    glClearColor(1.0f,1.0f,1.0f,1.0f);
    glColor3f(0.0f,0.0f,0.0f);
    glPointSize(4.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0f,screenWidth,0.0f,screenHeight);
}
void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f,0.0f,0.0f);
    glVertex2i(2,2);
    glVertex2i(510,2);
    glVertex2i(2,510);
    glColor3f(0.0f,1.0f,0.0f);
    glVertex2i(510,2);
    glVertex2i(510,510);
    glVertex2i(2,510);
    glEnd();
    glFlush();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowSize(screenWidth,screenHeight);
    glutInitWindowPosition(100,100);
    glutCreateWindow("OpenGL");
    glutDisplayFunc(display);
    init();
    glutMainLoop();
    return 0;
}
