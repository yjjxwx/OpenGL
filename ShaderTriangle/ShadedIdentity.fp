#version 330

flat in vec4 vVaryingColor;
out vec4 vFragColor;
void main(void){
	vFragColor = vVaryingColor;
}