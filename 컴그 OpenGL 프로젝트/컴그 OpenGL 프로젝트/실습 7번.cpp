#define _CRT_SECURE_NO_WARNINGS //--- 프로그램 맨 앞에 선언할 것
#include "library.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>

#define WINCX 800
#define WINCY 600

enum vertex_attribute {
	VER_POS, VER_COLOR, VER_END
};

enum Shapes {
	SHAPE_POINT, SHAPE_LINE, SHAPE_TRIANGLE, SHAPE_SQUARE
};

enum Dir {
	DIR_UP, DIR_DOWN, DIR_RIGHT, DIR_LEFT
};


struct Shape {
	Shapes Shape;
	GLuint vao;
	GLuint vbo[VER_END];
	GLenum drawMode;
	float color[3];
	float pos[3];
	float size;
	int vertexCount;
	
};

std::vector<Shape> vShapes;

char* filetobuf(const char* file);
//void Init_Buffer();
void Make_vertexShaders();
void Make_fragmentShaders();
void Make_shaderProgram();
void Add_Shape(Shapes ShapeName);

void MoveShape(Dir dir);

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
	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	glewInit();
	//--- 세이더 읽어와서 세이더 프로그램 만들기
	Make_shaderProgram();
	//--- 세이더 프로그램 만들기
	glutDisplayFunc(DrawScene); //--- 출력 콜백 함수
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);

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

	for (const auto& shape : vShapes) {
		glBindVertexArray(shape.vao); // 도형의 VAO 바인딩
		glDrawArrays(shape.drawMode, 0, shape.vertexCount); // 각 도형의 그리기
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


//void Init_Buffer() {
//
//	glGenVertexArrays(1, &vao);
//	//버텍스 배열 생성(생성할 VAO 개수, VAO를 저장할 배열의 이름)
//	glBindVertexArray(vao);
//	//VAO를 바인드함
//	glGenBuffers(VER_END, vbo);
//	//2개의 vbo 할당
//	glBindBuffer(GL_ARRAY_BUFFER,vbo[VER_POS]);
//	//VBO 바인드 하되 첫번쨰 포지션이라고 알려줌
//	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), trishape, GL_STATIC_DRAW);
//	//버퍼 오브젝트의 데이터를 생성 (버퍼타입, 버퍼 오브젝트의 크기, 저장할 데이터의 포인터, 사용할 패턴)
//	glVertexAttribPointer(VER_POS,3,GL_FLOAT,GL_FALSE,0,0);
//	//정점 속성 설정-버텍스 속성 데이터의 배열 정의(설정할 버텍스의 인덱스값(셰이더의 Layout), 버텍스 속성의 크기(vec3 = 3),
//	//데이터 타입,데이터를 정규화 할지 말지, 버텍스끼리의 공백, 데이터가 시작하는 위치의 오프셋)
//	glEnableVertexAttribArray(VER_POS);
//	//인덱스 0번 사용가능
//
//	glBindBuffer(GL_ARRAY_BUFFER, vbo[VER_COLOR]);
//	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
//	glVertexAttribPointer(VER_COLOR, 3, GL_FLOAT, GL_FALSE, 0, 0);
//	glEnableVertexAttribArray(VER_COLOR);
//
//
//}

GLvoid Keyboard(unsigned char key, int x, int y)
{

	switch (key) {
	case 'p':
		Add_Shape(SHAPE_POINT);
		break;
	case 'l':
		Add_Shape(SHAPE_LINE);
		break;
	case't':
		Add_Shape(SHAPE_TRIANGLE);
		break;
	case'r':
		Add_Shape(SHAPE_SQUARE);
		break;
	case 'w':
		MoveShape(DIR_UP);
		break;
	case 'a':
		MoveShape(DIR_LEFT);
		break;
	case 's':
		MoveShape(DIR_DOWN);
		break;
	case 'd':
		MoveShape(DIR_RIGHT);
		break;
	case 'c':
		vShapes.clear();
		break;

	case 'q':
		exit(0);
		break;
	}
	glutPostRedisplay();
}

void Add_Shape(Shapes ShapeName) {

	Shape shape;
	shape.Shape = ShapeName;
	shape.size = 0.1f;
	shape.pos[0] = (float)(rand() % WINCX) / WINCX * 2 - 1; 
	shape.pos[1] = (float)(rand() % WINCY) / WINCY * 2 - 1; 
	shape.pos[2] = 0.0f;
	shape.color[0] = (float)(rand() % 100) / 100;
	shape.color[1] = (float)(rand() % 100) / 100;
	shape.color[2] = (float)(rand() % 100) / 100;

	switch(ShapeName){
	case SHAPE_POINT:
		shape.vertexCount = 1;
		shape.drawMode = GL_POINTS;
		break;
	case SHAPE_LINE:
		shape.vertexCount = 2;
		shape.drawMode = GL_LINES;
		break;
	case SHAPE_TRIANGLE:
		shape.vertexCount = 3;
		shape.drawMode = GL_TRIANGLES;
		break;
	case SHAPE_SQUARE:
		shape.vertexCount = 6;
		shape.drawMode = GL_TRIANGLES;
		break;
	default:
		break;
	}

	glGenVertexArrays(1, &shape.vao);
	glGenBuffers(VER_END, shape.vbo);
	glBindVertexArray(shape.vao);

	GLfloat* vertices = new GLfloat[shape.vertexCount * 3];
	GLfloat* colors = new GLfloat[shape.vertexCount * 3];

	switch (ShapeName) {
	case SHAPE_POINT:
		vertices[0] = shape.pos[0];
		vertices[1] = shape.pos[1];
		vertices[2] = shape.pos[2];
		colors[0] = shape.color[0];
		colors[1] = shape.color[1];
		colors[2] = shape.color[2];
		break;

	case SHAPE_LINE:
		vertices[0] = shape.pos[0] - shape.size;
		vertices[1] = shape.pos[1];
		vertices[2] = shape.pos[2];

		vertices[3] = shape.pos[0] + shape.size;
		vertices[4] = shape.pos[1];
		vertices[5] = shape.pos[2];

		colors[0] = colors[3] = shape.color[0]; 
		colors[1] = colors[4] = shape.color[1]; 
		colors[2] = colors[5] = shape.color[2]; 
		break;

	case SHAPE_TRIANGLE:
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
		break;

	case SHAPE_SQUARE:
		vertices[0] = shape.pos[0] - shape.size; 
		vertices[1] = shape.pos[1] - shape.size;
		vertices[2] = shape.pos[2];

		vertices[3] = shape.pos[0] + shape.size; 
		vertices[4] = shape.pos[1] - shape.size;
		vertices[5] = shape.pos[2];

		vertices[6] = shape.pos[0] - shape.size; 
		vertices[7] = shape.pos[1] + shape.size;
		vertices[8] = shape.pos[2];

		vertices[9] = shape.pos[0] + shape.size; 
		vertices[10] = shape.pos[1] - shape.size;
		vertices[11] = shape.pos[2];

		vertices[12] = shape.pos[0] + shape.size;
		vertices[13] = shape.pos[1] + shape.size;
		vertices[14] = shape.pos[2];

		vertices[15] = shape.pos[0] - shape.size;
		vertices[16] = shape.pos[1] + shape.size;
		vertices[17] = shape.pos[2];

		colors[0] = colors[3] = colors[6] = colors[9] = shape.color[0];
		colors[1] = colors[4] = colors[7] = colors[10] = shape.color[1];
		colors[2] = colors[5] = colors[8] = colors[11] = shape.color[2];
		break;

	default:
		break;
	}

	glBindBuffer(GL_ARRAY_BUFFER, shape.vbo[VER_POS]);
	glBufferData(GL_ARRAY_BUFFER, shape.vertexCount * 3 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(VER_POS, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(VER_POS);

	glBindBuffer(GL_ARRAY_BUFFER, shape.vbo[VER_COLOR]);
	glBufferData(GL_ARRAY_BUFFER, shape.vertexCount * 3 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(VER_COLOR, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(VER_COLOR);

	delete[] vertices;
	delete[] colors;

	vShapes.push_back(shape);

}

void MoveShape(Dir dir)
{
	if (vShapes.empty()) return;

	int randIndex = rand() % vShapes.size();
	Shape& shape = vShapes[randIndex];

	float moveAmount = 0.1f;

	switch (dir) {
	case DIR_UP:
		shape.pos[1] += moveAmount; 
		break;
	case DIR_DOWN:
		shape.pos[1] -= moveAmount; 
		break;
	case DIR_RIGHT:
		shape.pos[0] += moveAmount; 
		break;
	case DIR_LEFT:
		shape.pos[0] -= moveAmount; 
		break;
	default:
		break;
	}

	glBindVertexArray(shape.vao);
	glBindBuffer(GL_ARRAY_BUFFER, shape.vbo[VER_POS]);

	GLfloat* vertices = new GLfloat[shape.vertexCount * 3];

	switch (shape.Shape) {
	case SHAPE_POINT:
		vertices[0] = shape.pos[0];
		vertices[1] = shape.pos[1];
		vertices[2] = shape.pos[2];
		break;

	case SHAPE_LINE:
		vertices[0] = shape.pos[0] - shape.size;
		vertices[1] = shape.pos[1];
		vertices[2] = shape.pos[2];

		vertices[3] = shape.pos[0] + shape.size;
		vertices[4] = shape.pos[1];
		vertices[5] = shape.pos[2];
		break;

	case SHAPE_TRIANGLE:
		vertices[0] = shape.pos[0];
		vertices[1] = shape.pos[1] + shape.size;
		vertices[2] = shape.pos[2];

		vertices[3] = shape.pos[0] - shape.size;
		vertices[4] = shape.pos[1] - shape.size;
		vertices[5] = shape.pos[2];

		vertices[6] = shape.pos[0] + shape.size; 
		vertices[7] = shape.pos[1] - shape.size;
		vertices[8] = shape.pos[2];
		break;

	case SHAPE_SQUARE:
		vertices[0] = shape.pos[0] - shape.size; 
		vertices[1] = shape.pos[1] - shape.size;
		vertices[2] = shape.pos[2];

		vertices[3] = shape.pos[0] + shape.size; 
		vertices[4] = shape.pos[1] - shape.size;
		vertices[5] = shape.pos[2];

		vertices[6] = shape.pos[0] - shape.size; 
		vertices[7] = shape.pos[1] + shape.size;
		vertices[8] = shape.pos[2];

		vertices[9] = shape.pos[0] + shape.size; 
		vertices[10] = shape.pos[1] - shape.size;
		vertices[11] = shape.pos[2];

		vertices[12] = shape.pos[0] + shape.size;
		vertices[13] = shape.pos[1] + shape.size;
		vertices[14] = shape.pos[2];

		vertices[15] = shape.pos[0] - shape.size;
		vertices[16] = shape.pos[1] + shape.size;
		vertices[17] = shape.pos[2];
		break;

	default:
		break;
	}

	glBufferData(GL_ARRAY_BUFFER, shape.vertexCount * 3 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	delete[] vertices;

	glutPostRedisplay();

}
