#define _CRT_SECURE_NO_WARNINGS //--- ���α׷� �� �տ� ������ ��
#include "library.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <cmath>

#define M_PI 3.14
#define WINCX 800
#define WINCY 600

enum vertex_attribute {
	VER_POS, VER_COLOR, VER_END
};

enum Pos {
	POS_X, POS_Y, POS_Z, POS_END
};

enum Color {
	COLOR_R, COLOR_G, COLOR_B, COLOR_END
};

struct Point {

	float fPos[POS_END];
	float fColor[COLOR_END];
	
};



struct Spiral {

	bool isLine;
	float fPos[POS_END];
	float fSpeed;
	GLuint vao;
	GLuint vbo[VER_END];
	std::vector<Point*> vPoints;
	void Add_Points() {

		Point* point = new Point;
		point->fColor[COLOR_R] = 1.f;
		point->fColor[COLOR_G] = 1.f;
		point->fColor[COLOR_B] = 1.f;
		point->fPos[POS_X] = fPos[POS_X];
		point->fPos[POS_Y] = fPos[POS_Y];
		point->fPos[POS_Z] = fPos[POS_Z];
		
		vPoints.push_back(point);
	}

	void Delete_Points() {

		for (auto& iter : vPoints) {
			delete(iter);
		}
	}


};

bool DrawMode = true;

const int maxShapesPerQuadrant = 3;
std::vector<Spiral*> vSpiral;

char* filetobuf(const char* file);
void Make_vertexShaders();
void Make_fragmentShaders();
void Make_shaderProgram();


void Add_Spire(int count,int PosX,int PosY);
void Mouse(int, int, int, int);
void TimerFunction(int value);
void Bind_Shape();


GLfloat normalizeX(int x) {
	return (x / 400.0f) - 1.0f;
}
GLfloat normalizeY(int y) {
	return -((y / 300.0f) - 1.0f);
}
GLvoid DrawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);

//--- �ʿ��� ���� ����
GLuint shaderProgramID; //--- ���̴� ���α׷� �̸�
GLuint vertexShader; //--- ���ؽ� ���̴� ��ü
GLuint fragmentShader; //--- �����׸�Ʈ ���̴� ��ü




//--- ���� �Լ�
void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
{
	//--- ������ �����ϱ�
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WINCX, WINCY);
	glutCreateWindow("�ǽ� 7");

	glutTimerFunc(10, TimerFunction, 0);
	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	glewInit();
	//--- ���̴� �о�ͼ� ���̴� ���α׷� �����
	Make_shaderProgram();
	//--- ���̴� ���α׷� �����
	glutDisplayFunc(DrawScene); //--- ��� �ݹ� �Լ�
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMainLoop();
}

//--- ��� �ݹ� �Լ�
void DrawScene() //--- glutDisplayFunc()�Լ��� ����� �׸��� �ݹ� �Լ�
{

	GLfloat rColor, gColor, bColor;
	rColor = bColor = 0.0;
	gColor = 0.0; //--- ������ �ʷϻ����� ����
	glClearColor(rColor, gColor, bColor, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaderProgramID);
	glutSwapBuffers();
}
//--- �ٽñ׸��� �ݹ� �Լ�
GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
{
	glViewport(0, 0, w, h);
}

void Make_vertexShaders()
{
	GLchar* vertexSource;
	vertexSource = filetobuf("vertex.glsl"); //--- ���ؽ����̴� �о����
	//--- ���ؽ� ���̴� �о� �����ϰ� ������ �ϱ�
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader error\n" << errorLog << std::endl;
		return;
	}
}
//--- �����׸�Ʈ ���̴� ��ü �����
void Make_fragmentShaders()
{
	GLchar* fragmentSource;
	fragmentSource = filetobuf("fragment.glsl"); //--- �����׸�Ʈ���̴� �о����
	//--- �����׸�Ʈ ���̴� �о� �����ϰ� �������ϱ�
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR: fragment shader error\n" << errorLog << std::endl;
		return;
	}
}


void Make_shaderProgram()
{
	Make_vertexShaders(); //--- ���ؽ� ���̴� �����
	Make_fragmentShaders(); //--- �����׸�Ʈ ���̴� �����

	shaderProgramID = glCreateProgram(); //--- ���̴� ���α׷� �����
	GLint result;
	GLchar errorLog[512];

	glAttachShader(shaderProgramID, vertexShader); //--- ���̴� ���α׷��� ���ؽ� ���̴� ���̱�
	glAttachShader(shaderProgramID, fragmentShader); //--- ���̴� ���α׷��� �����׸�Ʈ ���̴� ���̱�
	glLinkProgram(shaderProgramID); //--- ���̴� ���α׷� ��ũ�ϱ�


	glDeleteShader(vertexShader); //--- ���̴� ���α׷��� ��ũ�Ͽ� ���̴� ��ü ��ü�� ���� ����
	glDeleteShader(fragmentShader);

	glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &result); // ---���̴��� �� ����Ǿ����� üũ�ϱ�
	if (!result) {
		glGetProgramInfoLog(shaderProgramID, 512, NULL, errorLog);
		std::cerr << "ERROR: shader program ���� ����\n" << errorLog << std::endl;
		return;
	}
}


char* filetobuf(const char* file)
{
	FILE* fptr;
	long length;
	char* buf;
	fptr = fopen(file, "rb"); // Open file for reading
	if (!fptr) // Return NULL on failure
		return NULL;
	fseek(fptr, 0, SEEK_END); // Seek to the end of the file
	length = ftell(fptr); // Find out how many bytes into the file we are
	buf = (char*)malloc(length + 1); // Allocate a buffer for the entire length of the file and a null terminator
	fseek(fptr, 0, SEEK_SET); // Go back to the beginning of the file
	fread(buf, length, 1, fptr); // Read the contents of the file in to the buffer
	fclose(fptr); // Close the file
	buf[length] = 0; // Null terminator
	return buf; // Return the buffer
}

GLvoid Keyboard(unsigned char key, int x, int y)
{

	switch (key) {
	case 'p':
		break;

	case 'l':
		
		break;
	case '2':

		break;
	case '3':

		break;
	case '4':

		break;
	case '5':
		break;

	case 'q':
		exit(0);
		break;
	}
	glutPostRedisplay();
}

void Add_Shape(int x, int y) {
	
	for (auto& iter : vSpiral) {


		glGenVertexArrays(1, &iter->vao);
		glGenBuffers(VER_END, iter->vbo);
		glBindVertexArray(iter->vao);


		
		GLfloat* vertices = new GLfloat[POS_END];
		GLfloat* colors = new GLfloat[COLOR_END];
		
		vertices[POS_X] = iter->fPos[POS_X];
		vertices[POS_Y] = iter->fPos[POS_Y];
		vertices[POS_Z] = iter->fPos[POS_Z];

		colors[COLOR_R] = 1.f;
		colors[COLOR_G] = 1.f;
		colors[COLOR_B] = 1.f;


		glBindBuffer(GL_ARRAY_BUFFER, iter->vbo[VER_POS]);
		glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(VER_POS, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(VER_POS);

		glBindBuffer(GL_ARRAY_BUFFER, iter->vbo[VER_COLOR]);
		glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
		glVertexAttribPointer(VER_COLOR, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(VER_COLOR);

		delete[] vertices;
		delete[] colors;

	}




}
//
//void Bind_Shape() {
//	for (auto iter : vTriangles) {
//		glBindVertexArray(iter.vao); // ���� VAO ���ε�
//
//
//		GLfloat* vertices = new GLfloat[3 * 3];
//		GLfloat* colors = new GLfloat[3 * 3];
//
//
//		vertices[0] = iter.pos[0];
//		vertices[1] = iter.pos[1] + iter.size;
//		vertices[2] = iter.pos[2];
//
//		vertices[3] = iter.pos[0] - iter.size;
//		vertices[4] = iter.pos[1] - iter.size;
//		vertices[5] = iter.pos[2];
//
//		vertices[6] = iter.pos[0] + iter.size;
//		vertices[7] = iter.pos[1] - iter.size;
//		vertices[8] = iter.pos[2];
//
//		colors[0] = colors[3] = colors[6] = iter.color[0];
//		colors[1] = colors[4] = colors[7] = iter.color[1];
//		colors[2] = colors[5] = colors[8] = iter.color[2];
//
//
//		glBindBuffer(GL_ARRAY_BUFFER, iter.vbo[VER_POS]);
//		glBufferData(GL_ARRAY_BUFFER, 3 * 3 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
//		glVertexAttribPointer(VER_POS, 3, GL_FLOAT, GL_FALSE, 0, 0);
//		glEnableVertexAttribArray(VER_POS);
//
//		glBindBuffer(GL_ARRAY_BUFFER, iter.vbo[VER_COLOR]);
//		glBufferData(GL_ARRAY_BUFFER, 3 * 3 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
//		glVertexAttribPointer(VER_COLOR, 3, GL_FLOAT, GL_FALSE, 0, 0);
//		glEnableVertexAttribArray(VER_COLOR);
//	}
//}

void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		Add_Spire(x, y, 1);
	}
}



void TimerFunction(int value)
{
	Bind_Shape();
	glutPostRedisplay();
	glutTimerFunc(10, TimerFunction, 1);
}

void Add_Spire(int count, int PosX, int PosY)
{

	for (int i = 0; i < count + 1; ++i) {
		Spiral* spire = new Spiral;
		if (i == 0) {
			spire->fPos[POS_X] = normalizeX(rand() % WINCX);
			spire->fPos[POS_Y] = normalizeY(rand() % WINCY);
			spire->fPos[POS_Z] = 0.f;
		}
		else {
			spire->fPos[POS_X] = PosX;
			spire->fPos[POS_Y] = PosY;
			spire->fPos[POS_Z] = 0.f;
		}
		spire->fSpeed = 0.1f;
		spire->isLine = DrawMode;
		vSpiral.push_back(spire);
	}
}
