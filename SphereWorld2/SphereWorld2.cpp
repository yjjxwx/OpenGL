#include <GLTools.h>
#include <GLShaderManager.h>
#include <GL/glut.h>
#include <GLFrustum.h>
#include <math3d.h>
#include <StopWatch.h>
#include <GLMatrixStack.h>
#include <GLGeometryTransform.h>
#define NUM_SPHERES 50
GLTriangleBatch	torusBatch;
GLBatch floorBatch;
GLTriangleBatch sphereBatch;
GLShaderManager	shaderManager;
GLFrustum viewFrustum; 
GLMatrixStack modelViewMatrix;
GLMatrixStack projectionMatrix;
GLGeometryTransform transformPipeLine;
GLFrame cameraFrame;
GLFrame spheres[NUM_SPHERES];
M3DVector4f vLightPos = {0.0f, 10.0f, 5.0f, 1.0f};
M3DVector4f vLightEyePos;

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
	gltMakeTorus(torusBatch,0.4f, 0.15f, 40, 40);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    floorBatch.Begin(GL_LINES, 324);
    for(GLfloat x = -20.0; x <= 20.0f; x+= 0.5) {
        floorBatch.Vertex3f(x, -0.55f, 20.0f);
        floorBatch.Vertex3f(x, -0.55f, -20.0f);
        
        floorBatch.Vertex3f(20.0f, -0.55f, x);
        floorBatch.Vertex3f(-20.0f, -0.55f, x);
    }
    floorBatch.End();
    for(int i = 0; i < NUM_SPHERES; ++i){
    	GLfloat x = ((GLfloat)((rand() % 400) - 200) * 0.1f);
		GLfloat z = ((GLfloat)((rand() % 400) - 200) * 0.1f);
		spheres[i].SetOrigin(x,0.0f,z);
    }
    gltMakeSphere(sphereBatch, 0.2, 30, 30);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
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
	m3dTransformVector4(vLightEyePos, vLightPos, mCamera);
	modelViewMatrix.PushMatrix(mCamera);
	shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, 
		transformPipeLine.GetModelViewMatrix(),
		transformPipeLine.GetProjectionMatrix(),
		vLightEyePos,
		vFloorColor);
	floorBatch.Draw();

	for(int i = 0; i < NUM_SPHERES; ++i){
		modelViewMatrix.PushMatrix();
		modelViewMatrix.Rotate(yRot*-0.2f,0.0f,1.0f,0.0f);
		modelViewMatrix.MultMatrix(spheres[i]);
		shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, 
			transformPipeLine.GetModelViewMatrix(),
			transformPipeLine.GetProjectionMatrix(),
			vLightEyePos,
			vSphereColor);
		sphereBatch.Draw();
		modelViewMatrix.PopMatrix();	
	}

	modelViewMatrix.Translate(0.0f,0.0f, -2.50f);
	modelViewMatrix.PushMatrix();

	modelViewMatrix.Rotate(yRot,0.0f,1.0f,0.0f);
	shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, 
		transformPipeLine.GetModelViewMatrix(),
		transformPipeLine.GetProjectionMatrix(),
		vLightEyePos,
		vTorusColor);
	torusBatch.Draw();
	modelViewMatrix.PopMatrix();
	modelViewMatrix.Rotate(yRot*-2.0f,0.0f,1.0f,0.0f);
	modelViewMatrix.Translate(0.8f,0.0f,0.0f);
	shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, 
		transformPipeLine.GetModelViewMatrix(),
		transformPipeLine.GetProjectionMatrix(),
		vLightEyePos,
		vSphereColor);
	sphereBatch.Draw();
	modelViewMatrix.PopMatrix();
	modelViewMatrix.PopMatrix();
	glutSwapBuffers();
	glutPostRedisplay();
}

void SpecialKeys(int key, int x, int y){
	GLfloat linear = 0.08f;
	GLfloat angular = (GLfloat)(m3dDegToRad(3.0f));
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

