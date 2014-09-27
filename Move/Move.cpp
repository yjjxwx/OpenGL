// Triangle.cpp
// Our first OpenGL program that will just draw a triangle on the screen.
#include <GLTools.h>
#include <GLShaderManager.h>    // Shader Manager Class

#ifdef __APPLE__
#include <glut/glut.h>          // OS X version of GLUT
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>            // Windows FreeGlut equivalent
#endif

GLBatch	squareBatch;
GLShaderManager	shaderManager;
// Load up a triangle
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
	glViewport(0, 0, w, h);
}


///////////////////////////////////////////////////////////////////////////////
// This function does any needed initialization on the rendering context. 
// This is the first opportunity to do any OpenGL related tasks.
void SetupRC(){
	// Blue background
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f );
	shaderManager.InitializeStockShaders();
	squareBatch.Begin(GL_TRIANGLE_FAN, 4);
	squareBatch.CopyVertexData3f(vVerts);
	squareBatch.End();
}



///////////////////////////////////////////////////////////////////////////////
// Called to draw scene
void RenderScene(void){
	// Clear the window with current clearing color
// | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f );
	GLfloat vRed[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	shaderManager.UseStockShader(GLT_SHADER_IDENTITY, vRed);
	squareBatch.Draw();

	// Perform the buffer swap to display back buffer
	glutSwapBuffers();

	/*BounceFunction();
	glutPostRedisplay();*/
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
	if(blockX < -1.0f){
		blockX = -1.0f;
	}
	if(blockX > (1.0f - blockSize*2)){
		blockX = 1.0f - blockSize*2;
	}
	if(blockY < -1.0f + blockSize * 2){
		blockY = -1.0f + blockSize * 2;
	}
	if(blockY > 1.0f){
		blockY = 1.0f;
	}

	vVerts[0] = blockX;
	vVerts[1] = blockY - blockSize * 2;

	vVerts[3] = blockX + blockSize * 2;
	vVerts[4] = blockY - blockSize * 2;

	vVerts[6] = blockX + blockSize * 2;
	vVerts[7] = blockY;

	vVerts[9] = blockX;
	vVerts[10] = blockY;

	squareBatch.CopyVertexData3f(vVerts);
	glutPostRedisplay();
}


///////////////////////////////////////////////////////////////////////////////
// Main entry point for GLUT based programs
int main(int argc, char* argv[]){
	gltSetWorkingDirectory(argv[0]);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
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
