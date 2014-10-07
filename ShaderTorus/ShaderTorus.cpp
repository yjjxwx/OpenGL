#include <GLTools.h>
#include <GLShaderManager.h>
#include <GL/gl.h>
#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GLFrustum.h>
#include <GLFrame.h>
#include <math3d.h>
#include <GLMatrixStack.h>
#include <GLGeometryTransform.h>
#include <StopWatch.h>

GLTriangleBatch torusBatch;
GLFrustum viewFrustum;
GLFrame viewFrame;
GLMatrixStack modelViewMatrix;
GLMatrixStack projectionMatrix;
GLGeometryTransform transformPipeLine;
GLfloat ratio = 0.5;
GLuint mFlatShader = -1;
GLint mLocMVP = -1;
GLint mLocColor = -1;

void SetupRC(){
	glClearColor(0.0f,0.0f,0.0f,1.0f);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	viewFrame.MoveForward(4.0f);
	gltMakeTorus(torusBatch, 0.8f, 0.25f, 52, 26);
	mFlatShader = gltLoadShaderPairWithAttributes("ShadedFlat.vp", "ShadedFlat.fp", 1, GLT_ATTRIBUTE_VERTEX, "vVertex");
	mLocMVP = glGetUniformLocation(mFlatShader,"mvpMatrix");
	mLocColor = glGetUniformLocation(mFlatShader,"vColorValue");
}

void ChangeSize(GLint width, GLint height){
	if(height == 0){
		height = 1;
	}
	ratio = (GLfloat)width/(GLfloat)height;
	glViewport(0,0,width,height);
	viewFrustum.SetPerspective(35.0f,ratio,1.0f, 100.0f);
	projectionMatrix.LoadMatrix(viewFrustum.GetProjectionMatrix());
	transformPipeLine.SetMatrixStacks(modelViewMatrix, projectionMatrix);
}

void ShutDownRC(){

}

void RenderScene(void){
	static CStopWatch rotTimer;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	modelViewMatrix.PushMatrix(viewFrame);
	modelViewMatrix.Rotate(rotTimer.GetElapsedSeconds() * 50.0f, 0.0f,1.0f,0.0f);
	GLfloat vColor[] = {0.1f, 0.1f, 1.0f, 1.0f};
	glUseProgram(mFlatShader);
	glUniform4fv(mLocColor, 1, vColor);
	glUniformMatrix4fv(mLocMVP, 1, GL_FALSE, transformPipeLine.GetModelViewProjectionMatrix());
	torusBatch.Draw();
	modelViewMatrix.PopMatrix();
	glutSwapBuffers();
	glutPostRedisplay();
}

int main(int argc, char* argv[]){
	gltSetWorkingDirectory(argv[0]);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH| GLUT_STENCIL);
	glutInitWindowSize(800, 600);
	glutCreateWindow("ShadedFlat");
    	glutReshapeFunc(ChangeSize);
    	glutDisplayFunc(RenderScene);

	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
		return 1;
	}
	SetupRC();
	glutMainLoop();
	ShutDownRC();
	return 0;
}

