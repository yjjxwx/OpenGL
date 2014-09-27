#include <GLTools.h>
#include <GLShaderManager.h>
#include <GL/glut.h>
#include <GLFrustum.h>
#include <math3d.h>
#include <StopWatch.h>

GLTriangleBatch	torusBatch;
GLShaderManager	shaderManager;
GLFrustum viewFrustum; 

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
	viewFrustum.SetPerspective(35.0f, (float)w/(float)h, 1.0f, 1000.0f);
}


///////////////////////////////////////////////////////////////////////////////
// This function does any needed initialization on the rendering context. 
// This is the first opportunity to do any OpenGL related tasks.
void SetupRC(){
	// Blue background
	glClearColor(0.8f, 0.8f, 0.8f, 1.0f );
	shaderManager.InitializeStockShaders();
	gltMakeTorus(torusBatch,0.4f, 0.15f, 30, 30);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}



///////////////////////////////////////////////////////////////////////////////
// Called to draw scene
void RenderScene(void){
	// Clear the window with current clearing color
// | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//Set up time based animation
	static CStopWatch rotTimer;
	float yRot = rotTimer.GetElapsedSeconds() * 60.0f;

	//Matrix variables
	M3DMatrix44f mTranslate, mRotate, mModelView, mModelViewProjection;

	//Create a translation matrix to move the torus back and into sight.
	m3dTranslationMatrix44(mTranslate,0.0f,0.0f,-2.5f);
	
	m3dRotationMatrix44(mRotate,m3dDegToRad(yRot),0.0f,1.0f,0.0f);
	//Add a rotation to the translation. store the result in mModelView.
	m3dMatrixMultiply44(mModelView,mTranslate, mRotate);

	m3dMatrixMultiply44(mModelViewProjection, viewFrustum.GetProjectionMatrix(), mModelView);

	GLfloat vBlack[] = {
		0.0f,0.0f,0.0f,1.0f
	};

	shaderManager.UseStockShader(GLT_SHADER_FLAT, mModelViewProjection, vBlack);
	torusBatch.Draw();
	glutSwapBuffers();
	glutPostRedisplay();
}

void SpecialKeys(int key, int x, int y){
	GLfloat stepSize = 0.025f;
	GLfloat blockX = vVerts[0];
	GLfloat blockY = vVerts[7];

	switch(key){
		case GLUT_KEY_UP:
			blockY += stepSize;
			break;
		case GLUT_KEY_DOWN:
			blockY -= stepSize;
			break;
		case GLUT_KEY_LEFT:
			blockX -= stepSize;
			break;
		case GLUT_KEY_RIGHT:
			blockX += stepSize;
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

