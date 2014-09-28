#include <GLTools.h>
#include <GLShaderManager.h>
#include <GL/glut.h>
#include <GLFrustum.h>
#include <math3d.h>
#include <StopWatch.h>
#include <GLMatrixStack.h>
#include <GLGeometryTransform.h>
GLTriangleBatch	torusBatch;
GLBatch floorBatch;
GLTriangleBatch sphereBatch;
GLShaderManager	shaderManager;
GLFrustum viewFrustum; 
GLMatrixStack modelViewMatrix;
GLMatrixStack projectionMatrix;
GLGeometryTransform transformPipeLine;
GLFrame cameraFrame;

GLfloat vVerts[] = { 
	-0.5f, -0.5f, 0.0f, 
	0.5f, -0.5f, 0.0f,
	0.5f, 0.5f, 0.0f,
	-0.5f, 0.5f, 0.0f};
GLfloat blockSize = 0.5f;

///////////////////////////////////////////////////////////////////////////////
// Window has changed size, or has just been created. In either case, we need
// to use the window dimensions to set the viewport and the projection matrix.
void ChangeSize(int w, int h){
	if(h == 0){
		h = 1;
	}
	glViewport(0, 0, w, h);
	viewFrustum.SetPerspective(35.0f, (float)w/(float)h, 1.0f, 100.0f);
	projectionMatrix.LoadMatrix(viewFrustum.GetProjectionMatrix());

	transformPipeLine.SetMatrixStacks(modelViewMatrix, projectionMatrix);
}


///////////////////////////////////////////////////////////////////////////////
// This function does any needed initialization on the rendering context. 
// This is the first opportunity to do any OpenGL related tasks.
void SetupRC(){
	// Blue background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
	shaderManager.InitializeStockShaders();
	gltMakeTorus(torusBatch,0.4f, 0.15f, 30, 30);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    floorBatch.Begin(GL_LINES, 324);
    for(GLfloat x = -20.0; x <= 20.0f; x+= 0.5) {
        floorBatch.Vertex3f(x, -0.55f, 20.0f);
        floorBatch.Vertex3f(x, -0.55f, -20.0f);
        
        floorBatch.Vertex3f(20.0f, -0.55f, x);
        floorBatch.Vertex3f(-20.0f, -0.55f, x);
        }
    floorBatch.End();

    gltMakeSphere(sphereBatch, 0.2, 16, 20);
}



///////////////////////////////////////////////////////////////////////////////
// Called to draw scene
void RenderScene(void){
	static GLfloat vFloorColor[] = {0.0f, 1.0f, 0.0f, 1.0f};
	static GLfloat vTorusColor[] = {1.0f, 0.0f, 0.0f, 1.0f};
	static GLfloat vSphereColor[] = {0.0f, 0.0f, 1.0f, 1.0f};

	//Set up time based animation
	static CStopWatch rotTimer;
	float yRot = rotTimer.GetElapsedSeconds() * 60.0f;
	// Clear the window with current clearing color
// | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	modelViewMatrix.PushMatrix();
	M3DMatrix44f mCamera;
	cameraFrame.GetCameraMatrix(mCamera);
	modelViewMatrix.PushMatrix(mCamera);
	shaderManager.UseStockShader(GLT_SHADER_FLAT, transformPipeLine.GetModelViewProjectionMatrix(), vFloorColor);
	floorBatch.Draw();

	modelViewMatrix.Translate(0.0f,0.0f, -2.50f);
	modelViewMatrix.PushMatrix();

	modelViewMatrix.Rotate(yRot,0.0f,1.0f,0.0f);
	shaderManager.UseStockShader(GLT_SHADER_FLAT,transformPipeLine.GetModelViewProjectionMatrix(),vTorusColor);
	torusBatch.Draw();
	modelViewMatrix.PopMatrix();
	modelViewMatrix.Rotate(yRot*-2.0f,0.0f,1.0f,0.0f);
	modelViewMatrix.Translate(0.8f,0.0f,0.0f);
	shaderManager.UseStockShader(GLT_SHADER_FLAT, transformPipeLine.GetModelViewProjectionMatrix(),
		vSphereColor);
	sphereBatch.Draw();
	modelViewMatrix.PopMatrix();
	modelViewMatrix.PopMatrix();
	glutSwapBuffers();
	glutPostRedisplay();
}

void SpecialKeys(int key, int x, int y){
	GLfloat linear = 0.01f;
	GLfloat angular = (GLfloat)(m3dDegToRad(0.5f));

	switch(key){
		case GLUT_KEY_UP:
			cameraFrame.MoveForward(linear);
			break;
		case GLUT_KEY_DOWN:
			cameraFrame.MoveForward(-linear);
			break;
		case GLUT_KEY_LEFT:
			cameraFrame.RotateWorld(angular, 0.0,1.0f,0.0f);
			break;
		case GLUT_KEY_RIGHT:
			cameraFrame.RotateWorld(-angular, 0.0,1.0f,0.0f);
			break;
	}
}


///////////////////////////////////////////////////////////////////////////////
// Main entry point for GLUT based programs
int main(int argc, char* argv[]){
	gltSetWorkingDirectory(argv[0]);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Triangle");
    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(RenderScene);
    glutSpecialFunc(SpecialKeys);
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
		return 1;
	}
	SetupRC();
	glutMainLoop();
	return 0;
}

