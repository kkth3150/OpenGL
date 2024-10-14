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

enum Dir {
	DIR_UP, DIR_DOWN, DIR_RIGHT, DIR_LEFT, DIR_LT, DIR_RT, DIR_RB, DIR_LB, DIR_END
};

enum Face {
	FACE_LT, FACE_RT, FACE_RB, FACE_LB, FACE_END
};

enum MoveMode {
	MODE_STATIC, MODE_DIAGONAL, MODE_STRAIGHT, MODE_SQUARESPIRAL, MODE_CIRCLESPIRAL, MODE_END
};


struct Triangle {
	GLuint vao;
	GLuint vbo[VER_END];
	float color[3];
	float pos[3];
	float size;
	bool isLine;
	float speed;
	Dir eDir;
	MoveMode eMoveMode;

	float cir_spiralSize = 0.05f;
	float  cir_angle = 0.0f;
	int  cir_step = 0;
	float  cir_distance = 0.05f;

	float square_movecount = 0.f;
	float square_moveRange = 0.9f;


};



std::vector<Triangle> vTriangles;
int countShapes[FACE_END];
const int maxShapesPerQuadrant = 3;
bool TriangleMode = true;

GLfloat normalizeX(int);
GLfloat normalizeY(int);

char* filetobuf(const char* file);
void Make_vertexShaders();
void Make_fragmentShaders();
void Make_shaderProgram();
void Add_Shape(int x, int y);
void Delete_Shape(Face eface);
void Mouse(int, int, int, int);
void MoveShape(Dir dir);
void Move_Triangles(MoveMode);
void TimerFunction(int value);
void Bind_Shape();
void Move_SquareSpiral();

Face which_face(int mx, int my);
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
	rColor = bColor = 1.0;
	gColor = 1.0; //--- 배경색을 초록색으로 설정
	glClearColor(rColor, gColor, bColor, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaderProgramID);

	for (const auto& shape : vTriangles) {
		glBindVertexArray(shape.vao); // 도형의 VAO 바인딩
		if (shape.isLine)
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 3); // 각 도형의 그리기
		else {
			glDrawArrays(GL_LINE_LOOP, 0, 3);
		}
	}

	for (const auto& shape : vTriangles) {
		Move_Triangles(shape.eMoveMode);
	}
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
	case 'a':
		TriangleMode = true;
		break;

	case 'b':
		TriangleMode = false;
		break;
	case '1':
		for (auto& shape : vTriangles) {
			shape.eMoveMode = MODE_DIAGONAL;
		}
		break;
	case '2':
		for (auto& shape : vTriangles) {
			shape.eMoveMode = MODE_STRAIGHT;
		}
		break;
	case '3':
		for (auto& shape : vTriangles) {
			shape.eDir = DIR_LEFT;
			shape.eMoveMode = MODE_SQUARESPIRAL;
		}
		break;
	case '4':
		for (auto& shape : vTriangles) {
			shape.eMoveMode = MODE_CIRCLESPIRAL;
		}
		break;

	case 'q':
		exit(0);
		break;
	}
	glutPostRedisplay();
}

void Add_Shape(int x, int y) {
	Face face = which_face(x, y);

	if (countShapes[face] > 2) {
		return;
	}
	else {
		countShapes[face]++;
	}

	Triangle shape;
	shape.size = 0.1f + (float)(rand()) / ((float)(RAND_MAX / 0.1f));
	shape.pos[0] = (float)(x % WINCX) / WINCX * 2 - 1;
	shape.pos[1] = -((float)(y % WINCY) / WINCY * 2 - 1);
	shape.pos[2] = 0.0f;
	shape.color[0] = (float)(rand() % 100) / 100;
	shape.color[1] = (float)(rand() % 100) / 100;
	shape.color[2] = (float)(rand() % 100) / 100;
	shape.isLine = TriangleMode;
	shape.speed = ((float)(rand() % 100) / 100) * 0.01f + 0.01f;
	shape.eDir = DIR_RB;
	shape.eMoveMode = MODE_STATIC;
	glGenVertexArrays(1, &shape.vao);
	glGenBuffers(VER_END, shape.vbo);
	glBindVertexArray(shape.vao);

	GLfloat* vertices = new GLfloat[3 * 3];
	GLfloat* colors = new GLfloat[3 * 3];


	vertices[0] = shape.pos[0];
	vertices[1] = shape.pos[1] + shape.size;
	vertices[2] = shape.pos[2];

	vertices[3] = shape.pos[0] - shape.size;
	vertices[4] = shape.pos[1] - shape.size;
	vertices[5] = shape.pos[2];

	vertices[6] = shape.pos[0] + shape.size;
	vertices[7] = shape.pos[1] - shape.size;
	vertices[8] = shape.pos[2];

	colors[0] = colors[3] = colors[6] = shape.color[0];
	colors[1] = colors[4] = colors[7] = shape.color[1];
	colors[2] = colors[5] = colors[8] = shape.color[2];

	glBindBuffer(GL_ARRAY_BUFFER, shape.vbo[VER_POS]);
	glBufferData(GL_ARRAY_BUFFER, 3 * 3 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(VER_POS, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(VER_POS);

	glBindBuffer(GL_ARRAY_BUFFER, shape.vbo[VER_COLOR]);
	glBufferData(GL_ARRAY_BUFFER, 3 * 3 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(VER_COLOR, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(VER_COLOR);

	delete[] vertices;
	delete[] colors;

	vTriangles.push_back(shape);

}

void Bind_Shape() {
	for (auto iter : vTriangles) {
		glBindVertexArray(iter.vao); // 기존 VAO 바인딩


		GLfloat* vertices = new GLfloat[3 * 3];
		GLfloat* colors = new GLfloat[3 * 3];


		vertices[0] = iter.pos[0];
		vertices[1] = iter.pos[1] + iter.size;
		vertices[2] = iter.pos[2];

		vertices[3] = iter.pos[0] - iter.size;
		vertices[4] = iter.pos[1] - iter.size;
		vertices[5] = iter.pos[2];

		vertices[6] = iter.pos[0] + iter.size;
		vertices[7] = iter.pos[1] - iter.size;
		vertices[8] = iter.pos[2];

		colors[0] = colors[3] = colors[6] = iter.color[0];
		colors[1] = colors[4] = colors[7] = iter.color[1];
		colors[2] = colors[5] = colors[8] = iter.color[2];


		glBindBuffer(GL_ARRAY_BUFFER, iter.vbo[VER_POS]);
		glBufferData(GL_ARRAY_BUFFER, 3 * 3 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(VER_POS, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(VER_POS);

		glBindBuffer(GL_ARRAY_BUFFER, iter.vbo[VER_COLOR]);
		glBufferData(GL_ARRAY_BUFFER, 3 * 3 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
		glVertexAttribPointer(VER_COLOR, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(VER_COLOR);
	}
}

void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		Delete_Shape(which_face(x, y));
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

		Add_Shape(x, y);
	}
}

Face which_face(int mx, int my) {
	int centerX = WINCX / 2;
	int centerY = WINCY / 2;

	int invertedMy = my;

	if (mx < centerX && invertedMy < centerY) {
		return FACE_LT;
	}
	else if (mx >= centerX && invertedMy < centerY) {
		return FACE_RT;
	}
	else if (mx >= centerX && invertedMy >= centerY) {
		return FACE_RB;
	}
	else if (mx < centerX && invertedMy >= centerY) {
		return FACE_LB;
	}

	return FACE_END;
}

void Delete_Shape(Face eface)
{

	auto it = vTriangles.begin();
	while (it != vTriangles.end()) {
		bool Delete = false;

		switch (eface) {
		case FACE_LT:
			if (it->pos[0] < 0 && it->pos[1] > 0) {
				Delete = true;
			}
			break;
		case FACE_RT:
			if (it->pos[0] > 0 && it->pos[1] > 0) {
				Delete = true;
			}
			break;
		case FACE_RB:
			if (it->pos[0] > 0 && it->pos[1] < 0) {
				Delete = true;
			}
			break;
		case FACE_LB:
			if (it->pos[0] < 0 && it->pos[1] < 0) {
				Delete = true;
			}
			break;
		default:
			break;

		}


		if (Delete) {

			glDeleteVertexArrays(1, &it->vao);
			glDeleteBuffers(VER_END, it->vbo);
			--countShapes[eface];
			it = vTriangles.erase(it); 
		}
		else {
			++it; 
		}


	}
	glutPostRedisplay();

}

void Move_Triangles(MoveMode eMode) {
	switch (eMode) {
	case MODE_STATIC:
		return;

	case MODE_DIAGONAL:
		for (auto& triangle : vTriangles) {

			switch (triangle.eDir) {
			case DIR_RB: 
				triangle.pos[0] += triangle.speed; 
				triangle.pos[1] -= triangle.speed; 
				break;
			case DIR_RT: 
				triangle.pos[0] += triangle.speed; 
				triangle.pos[1] += triangle.speed; 
				break;
			case DIR_LB:
				triangle.pos[0] -= triangle.speed; 
				triangle.pos[1] -= triangle.speed; 
				break;
			case DIR_LT:
				triangle.pos[0] -= triangle.speed; 
				triangle.pos[1] += triangle.speed; 
				break;
			default:
				break;
			}

			if (triangle.pos[0] > 1.0f) { 
				triangle.pos[0] = 1.0f; 
				if (triangle.eDir == DIR_RB) {
					triangle.eDir = DIR_LB; 
				}
				else if (triangle.eDir == DIR_RT) {
					triangle.eDir = DIR_LT; 
				}
			}
			else if (triangle.pos[0] < -1.0f) {
				triangle.pos[0] = -1.0f; 
				if (triangle.eDir == DIR_LB) {
					triangle.eDir = DIR_RB;
				}
				else if (triangle.eDir == DIR_LT) {
					triangle.eDir = DIR_RT; 
				}
			}

			if (triangle.pos[1] > 1.0f) {
				triangle.pos[1] = 1.0f;
				if (triangle.eDir == DIR_RT) {
					triangle.eDir = DIR_RB; 
				}
				else if (triangle.eDir == DIR_LT) {
					triangle.eDir = DIR_LB;
				}
			}
			else if (triangle.pos[1] < -1.0f) { 
				triangle.pos[1] = -1.0f;
				if (triangle.eDir == DIR_RB) {
					triangle.eDir = DIR_RT;
				}
				else if (triangle.eDir == DIR_LB) {
					triangle.eDir = DIR_LT;
				}
			}
		}
		break;

	case MODE_STRAIGHT:
		for (auto& triangle : vTriangles) {

			if (triangle.eDir == DIR_RB || triangle.eDir == DIR_RT) {
				triangle.pos[0] += triangle.speed; 
			}
			else {
				triangle.pos[0] -= triangle.speed; 
			}

			if (triangle.pos[0] > 1.0f) { 
				triangle.pos[0] = 1.0f; 
				triangle.pos[1] -= 0.1f; 
				triangle.eDir = DIR_LB; 
			}
			else if (triangle.pos[0] < -1.0f) { 
				triangle.pos[0] = -1.0f; 
				triangle.pos[1] -= 0.1f; 
				triangle.eDir = DIR_RB; 
			}

			if (triangle.pos[1] > 1.0f) {
				triangle.pos[1] = 1.0f;
			}
			else if (triangle.pos[1] < -1.0f) {
				triangle.pos[1] = -1.0f;
			}
		}
		break;

	case MODE_SQUARESPIRAL:

		for (auto& i : vTriangles) {
			switch (i.eDir) {
			case DIR_LEFT:
				i.pos[0] -= i.speed;
				i.square_movecount += i.speed;
				break;
			case DIR_DOWN:
				i.pos[1] -= i.speed;
				i.square_movecount += i.speed;
				break;
			case DIR_RIGHT:
				i.pos[0] += i.speed;
				i.square_movecount += i.speed;
				break;
			case DIR_UP:
				i.pos[1] += i.speed;
				i.square_movecount += i.speed;
				break;
			}

			if (i.square_moveRange <= i.square_movecount) {
				i.square_movecount = 0.f;

				i.square_moveRange *= 0.9f;

				if (i.square_moveRange < 0.1f) {
					i.square_moveRange = 0.1f;
				}

				switch (i.eDir) {
				case DIR_LEFT:
					i.eDir = DIR_DOWN; 
					break;
				case DIR_DOWN:
					i.eDir = DIR_RIGHT;
					break;
				case DIR_RIGHT:
					i.eDir = DIR_UP;
					break;
				case DIR_UP:
					i.eDir = DIR_LEFT; 
					break;
				}
			}
		}
		break;
	case MODE_CIRCLESPIRAL:
		for (auto& triangle : vTriangles) {

			float xOffset = cos(triangle.cir_angle) * triangle.cir_distance;
			float yOffset = sin(triangle.cir_angle) * triangle.cir_distance;

			triangle.pos[0] += xOffset; 
			triangle.pos[1] += yOffset; 

			triangle.cir_angle += 0.1f;

			triangle.cir_distance *= 0.99f;

			if (triangle.cir_distance < 0.01f) {
				triangle.cir_distance = 0.01f;
			}

			if (triangle.pos[0] > 1.0f) { 
				triangle.pos[0] = 1.0f;
			}
			else if (triangle.pos[0] < -1.0f) { 
				triangle.pos[0] = -1.0f;
			}

			if (triangle.pos[1] > 1.0f) { 
				triangle.pos[1] = 1.0f;
			}
			else if (triangle.pos[1] < -1.0f) { 
				triangle.pos[1] = -1.0f;
			}
		}
		break;

	default:
		break;
	}
}


void TimerFunction(int value)
{
		Bind_Shape();
		glutPostRedisplay();
		glutTimerFunc(10, TimerFunction, 1);
}
