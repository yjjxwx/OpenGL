#include <iostream>

using namespace std;
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

const GLint screenWidth = 640;
const GLint screenHeight = 480;
GLfloat ratio = 0.9f;
void init(){
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glShadeModel(GL_SMOOTH);
    glViewport(0,0,screenWidth,screenHeight);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    ratio = (GLfloat)screenWidth/(GLfloat)screenHeight;
    // gluPerspective(45.0f,ratio,0.1f,100.f);
      gluOrtho2D(0.0f,screenWidth,0.0f,screenHeight);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
void display(){
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glBegin(GL_TRIANGLES);
		glColor3f(1.0f,0.0f,0.0f);
		glVertex2i(2,2);
		glVertex2i(screenWidth-2,2);
		glVertex2i(2,screenHeight-2);
		glColor3f(0.0f,1.0f,0.0f);
		glVertex2i(screenWidth-2,2);
		glVertex2i(screenWidth,screenHeight);
		glVertex2i(2,screenHeight);
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
