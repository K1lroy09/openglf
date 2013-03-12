#include <iostream>
#include <stdio.h>
#include <math.h>
#include <string>
#include <fstream>

#include <GL/glew.h>
#include <GL/glut.h>

#include "Camera.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////
// GL RELATED FUNCTIONS
///////////////////////////////////////////////////////////////////////////////
GLvoid printGLInfo();
GLvoid init(int* argc, char** argv);
GLvoid render();
GLvoid reshape(int width, int height);
GLvoid keyboardNormal(unsigned char key, int x, int y);
GLvoid keyboardSpecial(int key, int x, int y);

///////////////////////////////////////////////////////////////////////////////
// SHADER RELATED FUNCTIONS
///////////////////////////////////////////////////////////////////////////////
GLvoid processCompilationState(int compilationState);
GLvoid loadFile(const GLchar* filename, string& data);
GLuint loadShader(string& source, GLenum shaderType);
GLvoid getShader(GLuint* shaderID, const GLchar* filename, GLenum shaderType);
GLvoid initShaderProgram(const GLchar* vsFilename, const GLchar* fsFilename);
GLvoid cleanShaderProgram();

///////////////////////////////////////////////////////////////////////////////
// VARIABLES AND/OR OBJECTS/INSTANCES
///////////////////////////////////////////////////////////////////////////////
const char* vsFile = "media/shaders/vs.glsl";
const char* fsFile = "media/shaders/fs.glsl";

GLuint vs, fs, shaderProgramID;

GLuint vbo[3];
GLfloat vertices[] = {
					0.0f, 0.0f, 0.0f,
					1.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 0.0f,
					0.0f, 1.0f, 0.0f};
GLfloat colors[] = {
					1.0f, 0.0f, 0.0f,
					0.0f, 1.0f, 0.0f,
					0.0f, 0.0f, 1.0f,
					1.0f, 1.0f, 1.0f};
GLfloat normals[] = {
					0.0f, 0.0f, 1.0f,
					0.0f, 0.0f, 1.0f,
					0.0f, 0.0f, 1.0f,
					0.0f, 0.0f, 1.0f};
					
GLuint indices[] = {0, 1, 2, 0, 2, 3};

Camera camera;



///////////////////////////////////////////////////////////////////////////////
// MAIN
///////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
	init(&argc, argv);
		
	return 1;
}



///////////////////////////////////////////////////////////////////////////////
// GL RELATED FUNCTIONS
//////////////////////////////////////////////////////////////////////////////

GLvoid printGLInfo(){
	const GLubyte* version = glGetString(GL_VERSION);
	printf("GL_VERSION: %s\n", version);	
}

GLvoid init(int* argc, char** argv){
	//Glut
	glutInit(argc, argv);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(500, 500);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Arne Schreuder OpenGLF");
	
	//Glew
	//printGLInfo();
	GLenum error = glewInit();
	if(error != GLEW_OK){
		printf("Glew error: %s\n", glewGetErrorString(error));
		return;
	}
	
	//Shader program
	initShaderProgram(vsFile, fsFile);
	
	glGenBuffers(3, &vbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
	
	//Callbacks
	glutDisplayFunc(render);
	glutReshapeFunc(reshape);
	glutIdleFunc(render);
	glutKeyboardFunc(keyboardNormal);
	glutSpecialFunc(keyboardSpecial);
	
	//Mainloop
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
	
	glEnable(GL_DEPTH_TEST);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glColorPointer(3, GL_FLOAT, 0, 0);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glNormalPointer(GL_FLOAT, 0, 0);
	
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, &indices);
	
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisable(GL_DEPTH_TEST);
	
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
	
	camera.processKeys(key, x, y);
	
}

GLvoid keyboardSpecial(int key, int x, int y){
	camera.processSpecialKeys(key, x, y);
}

GLvoid loadFile(const GLchar* filename, string& data){
	printf("Loading file (%s) => ", filename);
	ifstream file(filename);
	if(!file.is_open()){
		printf("The file (%s) could not be opened.\n", filename);
	}	
	else{
		string temp;
		while(!file.eof()){
			getline(file, temp);
			data += temp;
			data += "\n";
		}
	}
	printf("DONE\n");
}



///////////////////////////////////////////////////////////////////////////////
// SHADER RELATED FUNCTIONS
///////////////////////////////////////////////////////////////////////////////

GLvoid processCompilationState(int shaderID){
		
	int compileStatus;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileStatus);
	
	if(compileStatus == GL_FALSE){  
		GLchar error[1000];
		glGetShaderInfoLog(shaderID, 1000, NULL, error);
		printf("Compilation failed: %s\n", error);
		cleanShaderProgram();
		exit(1);
	}
}

GLuint loadShader(string& source, GLenum shaderType){
	GLuint shaderID = glCreateShader(shaderType);
		
	const GLchar* csource = source.c_str();
	glShaderSource(shaderID, 1, &csource, NULL);	
	
	glCompileShader(shaderID);
	processCompilationState(shaderID);
		
	return shaderID;
}

GLvoid getShader(GLuint* shaderID, const GLchar* filename, GLenum shaderType){
	string source;
	loadFile(filename, source);
	*(shaderID) = loadShader(source, shaderType);
}

GLvoid initShaderProgram(const GLchar* vsFilename, const GLchar* fsFilename){
	getShader(&vs, vsFilename, GL_VERTEX_SHADER);
	getShader(&fs, fsFilename, GL_FRAGMENT_SHADER);
	
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








