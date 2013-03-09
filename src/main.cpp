#include <iostream>
#include <stdio.h>
#include <math.h>
#include <string>
#include <fstream>

#include <GL/glew.h>
#include <GL/glut.h>

#include "Camera.h"

using namespace std;

GLvoid init(int* argc, char** argv);
GLvoid render();
GLvoid reshape(int width, int height);
GLvoid keyboardNormal(unsigned char key, int x, int y);
GLvoid keyboardSpecial(int key, int x, int y);

GLvoid printGLInfo();
GLvoid loadFile(const GLchar* filename, string& data);
GLuint loadShader(string& source, GLenum shaderType);
GLvoid initShaderProgram(const GLchar* vsFilename, const GLchar* fsFilename);
GLvoid cleanShaderProgram();

float angle = 0.0f;

const float SAMPLE_SIZE = 0.05f;
const float SCALE_SIZE = 0.1f;

float P0[2] = {-1.0f, -1.0f};
float P1[2] = {0.0f, -1.0f};
float P2[2] = {0.0f, 1.0f};
float P3[2] = {1.0f, 1.0f};

const char* vsFile = "media/shaders/vs.glsl";
const char* fsFile = "media/shaders/fs.glsl";

GLuint vs, fs, shaderProgramID;

Camera camera;

int main(int argc, char** argv)
{
	init(&argc, argv);
		
	return 1;
}

GLvoid init(int* argc, char** argv){
	glutInit(argc, argv);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(500, 500);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Arne Schreuder OpenGLF");
	
	printGLInfo();
	GLenum error = glewInit();
	if(error != GLEW_OK){
		printf("Glew error: %s\n", glewGetErrorString(error));
		return;
	}
	initShaderProgram(vsFile, fsFile);
	
	glutDisplayFunc(render);
	glutReshapeFunc(reshape);
	glutIdleFunc(render);
	glutKeyboardFunc(keyboardNormal);
	glutSpecialFunc(keyboardSpecial);
	glutMainLoop();
}

GLvoid render(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(
		camera.pos[0], camera.pos[1], camera.pos[2],
		camera.lookAt[0], camera.lookAt[1], camera.lookAt[2],
		0.0f, 1.0f, 0.0f
	);
	glRotatef(angle, 0.0f, 1.0f, 0.0f);
	glLineWidth(5.0f);
	glTranslatef(0.0f, 0.0f, -5.0f);
	glColor3f(1.0f, 0.5f, 0.3f);
	glutSolidTeapot(1.0f);
	glEnable(GL_DEPTH_TEST);
	
	//angle += 0.5f;
	glutSwapBuffers();
}

GLvoid reshape(int width, int height){
	if(height == 0) height = 1;
	
	float ratio = ((float)width/height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, width, height);
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);		
}

GLvoid keyboardNormal(unsigned char key, int x, int y){
	if (key == 27){
		cleanShaderProgram();
		exit(0);
	}
		
	switch(key){
		case 'w' : camera.update(0.1f, 0.0, 0.0f, 0.0f); break;
		case 'a' : camera.update(0.0f, -0.1f, 0.0f, 0.0f); break;
		case 's' : camera.update(-0.1f, 0.0f, 0.0f, 0.0f); break;
		case 'd' : camera.update(0.0f, 0.1f, 0.0f, 0.0f); break;
	}	
	
}

GLvoid keyboardSpecial(int key, int x, int y){
	switch(key){
		case GLUT_KEY_UP : camera.update(0.0f, 0.0f, 5.0f, 0.0f); break;
		case GLUT_KEY_LEFT : camera.update(0.0f, 0.0f, 0.0f, -5.0f); break;
		case GLUT_KEY_DOWN : camera.update(0.0f, 0.0f, -5.0f, 0.0f); break;
		case GLUT_KEY_RIGHT : camera.update(0.0f, 0.0f, 0.0f, 5.0f); break;
	}
}

GLvoid loadFile(const GLchar* filename, string& data){
	printf("Loading file (%s)\n", filename);
	ifstream file(filename);
	if(!file.is_open()){
		printf("The file (%s) could not be opened.\n", filename);
	}	
	else{
		char temp[300];
		while(!file.eof()){
			file.getline(temp, 300);
			data += temp;
			data += "\n";
		}
	}
}

void printGLInfo(){
	const GLubyte* version = glGetString(GL_VERSION);
	printf("GL_VERSION: %s\n", version);	
}

GLuint loadShader(string& source, GLenum shaderType){
	GLuint shaderID = glCreateShader(shaderType);
		
	const GLchar* csource = source.c_str();
	glShaderSource(shaderID, 1, &csource, NULL);	
	
	glCompileShader(shaderID);
	
	int compileStatus;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileStatus);

	if(compileStatus == GL_FALSE){  
		GLchar error[1000];
		glGetShaderInfoLog(shaderID, 1000, NULL, error);
		printf("Compilation failed: %s\n", error);
	}
	else{
		printf("Compilation successful\n");
	}
	
	return shaderID;
}

GLvoid initShaderProgram(const GLchar* vsFilename, const GLchar* fsFilename){
	string source;
	loadFile(vsFilename, source);
	vs = loadShader(source, GL_VERTEX_SHADER);
	source = "";
	
	loadFile(fsFilename, source);
	fs = loadShader(source, GL_FRAGMENT_SHADER);
	
	shaderProgramID = glCreateProgram();
	glAttachShader(shaderProgramID, vs);
	glAttachShader(shaderProgramID, fs);
	glLinkProgram(shaderProgramID);
	glUseProgram(shaderProgramID);
}

GLvoid cleanShaderProgram(){
	glDetachShader(shaderProgramID, vs);
	glDetachShader(shaderProgramID, fs);
	
	glDeleteShader(vs);
	glDeleteShader(fs);
	
	glDeleteProgram(shaderProgramID);
}








