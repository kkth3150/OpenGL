#define _CRT_SECURE_NO_WARNINGS //--- 프로그램 맨 앞에 선언할 것
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

//--- 필요한 변수 선언
GLuint shaderProgramID; //--- 세이더 프로그램 이름
GLuint vertexShader; //--- 버텍스 세이더 객체
GLuint fragmentShader; //--- 프래그먼트 세이더 객체




//--- 메인 함수
void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
	//--- 윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WINCX, WINCY);
	glutCreateWindow("실습 7");

	glutTimerFunc(10, TimerFunction, 0);
	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	glewInit();
	//--- 세이더 읽어와서 세이더 프로그램 만들기
	Make_shaderProgram();
	//--- 세이더 프로그램 만들기
	glutDisplayFunc(DrawScene); //--- 출력 콜백 함수
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMainLoop();
}

//--- 출력 콜백 함수
void DrawScene() //--- glutDisplayFunc()함수로 등록한 그리기 콜백 함수
{

	GLfloat rColor, gColor, bColor;
	rColor = bColor = 0.0;
	gColor = 0.0; //--- 배경색을 초록색으로 설정
	glClearColor(rColor, gColor, bColor, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaderProgramID);
	glutSwapBuffers();
}
//--- 다시그리기 콜백 함수
GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
{
	glViewport(0, 0, w, h);
}

void Make_vertexShaders()
{
	GLchar* vertexSource;
	vertexSource = filetobuf("vertex.glsl"); //--- 버텍스세이더 읽어오기
	//--- 버텍스 세이더 읽어 저장하고 컴파일 하기
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
//--- 프레그먼트 세이더 객체 만들기
void Make_fragmentShaders()
{
	GLchar* fragmentSource;
	fragmentSource = filetobuf("fragment.glsl"); //--- 프래그먼트세이더 읽어오기
	//--- 프래그먼트 세이더 읽어 저장하고 컴파일하기
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
	Make_vertexShaders(); //--- 버텍스 세이더 만들기
	Make_fragmentShaders(); //--- 프래그먼트 세이더 만들기

	shaderProgramID = glCreateProgram(); //--- 세이더 프로그램 만들기
	GLint result;
	GLchar errorLog[512];

	glAttachShader(shaderProgramID, vertexShader); //--- 세이더 프로그램에 버텍스 세이더 붙이기
	glAttachShader(shaderProgramID, fragmentShader); //--- 세이더 프로그램에 프래그먼트 세이더 붙이기
	glLinkProgram(shaderProgramID); //--- 세이더 프로그램 링크하기


	glDeleteShader(vertexShader); //--- 세이더 프로그램에 링크하여 세이더 객체 자체는 삭제 가능
	glDeleteShader(fragmentShader);

	glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &result); // ---세이더가 잘 연결되었는지 체크하기
	if (!result) {
		glGetProgramInfoLog(shaderProgramID, 512, NULL, errorLog);
		std::cerr << "ERROR: shader program 연결 실패\n" << errorLog << std::endl;
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
//		glBindVertexArray(iter.vao); // 기존 VAO 바인딩
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
