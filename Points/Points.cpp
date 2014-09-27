#include <GLTools.h>
#include <GLShaderManager.h>
#include <GL/glut.h>

GLBatch	pointsBatch;
GLShaderManager	shaderManager;

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
	pointsBatch.Begin(GL_POINTS, 4);
	pointsBatch.CopyVertexData3f(vVerts);
	pointsBatch.End();
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
	GLfloat sizes[2];
	GLfloat step;
	glGetFloatv(GL_POINT_SIZE_RANGE,sizes);
	glGetFloatv(GL_POINT_SIZE_GRANULARITY, &step);
	GLfloat x = -1.0f;
	for(GLfloat s = sizes[0], x = -0.9f; s <= sizes[1]; s += step, x += 0.05f){
		glPointSize(s);
		pointsBatch.Begin(GL_POINTS, 1);
		GLfloat points[] = {
			x,0.0f,0.0f
		};
		pointsBatch.CopyVertexData3f(points);
		pointsBatch.End();
		pointsBatch.Draw();
	}
	// Perform the buffer swap to display back buffer
	glutSwapBuffers();
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

