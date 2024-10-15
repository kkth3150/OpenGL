#define _CRT_SECURE_NO_WARNINGS //--- ���α׷� �� �տ� ������ ��

#include "library.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>

#define SPACE1CENTERX  200.f 
#define SPACE1CENTERY  150.f
#define SPACE2CENTERX  600.f 
#define SPACE2CENTERY  150.f
#define SPACE3CENTERX  200.f
#define SPACE3CENTERY  450.f
#define SPACE4CENTERX  600.f
#define SPACE4CENTERY  450.f


enum vertex_attribute {
	VER_POS, VER_COLOR, VER_END
};

enum Pos {
	POS_X, POS_Y, POS_Z, POS_END
};

enum Color {
	COLOR_R, COLOR_G, COLOR_B, COLOR_END
};

enum LINEPOINT {

	POINT_LEFT, POINT_RIGHT, POINT_END

};

enum SPACE {
	SPACE_1, SPACE_2, SPACE_3, SPACE_4, SPACE_END
};

enum SHAPE {

	SHAPE_LINE, SHAPE_TRIANGEL, SHAPE_SQUARE, SHAPE_PENTAGON, SHAPE_END
};



struct Point {
	float fpos[POS_END];
};

struct Line {

	GLuint vao;
	GLuint vbo[VER_END];
	Point Point[POINT_END];
	float fcolor[COLOR_END];
};


struct Triangle{

	Point Point[3];
	GLuint vao;
	GLuint vbo[VER_END];
	float fcolor[COLOR_END];

};



GLfloat normalizeX(int x) {
	return (x / 400.0f) - 1.0f;
}
GLfloat normalizeY(int y) {
	return -((y / 300.0f) - 1.0f);
}


std::vector<Line*> vLines[SPACE_END];
std::vector<Triangle*> vTriangles[SPACE_END];

char* filetobuf(const char* file);

void Make_Line(Point Point1, Point Point2, float* color, SPACE space);
void Make_Triangle(Point point, float* color, SPACE space, bool Delay);
void Make_Square(Point point, float* color, SPACE space, bool Delay);
void Make_Pentagon(Point point, float* color, SPACE space);

void InitSet();
void Make_vertexShaders();
void Make_fragmentShaders();
GLuint Make_shaderProgram();
GLvoid DrawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
//--- �ʿ��� ���� ����
GLint width, height;
GLuint shaderProgramID; //--- ���̴� ���α׷� �̸�
GLuint vertexShader; //--- ���ؽ� ���̴� ��ü
GLuint fragmentShader; //--- �����׸�Ʈ ���̴� ��ü

//--- ���� �Լ�
void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
{
	width = 500;
	height = 500;
	//--- ������ �����ϱ�
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800,600);
	glutCreateWindow("Example1");
	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	glewInit();
	//--- ���̴� �о�ͼ� ���̴� ���α׷� �����
	Make_vertexShaders(); //--- ���ؽ� ���̴� �����
	Make_fragmentShaders(); //--- �����׸�Ʈ ���̴� �����
	shaderProgramID = Make_shaderProgram();
	//--- ���̴� ���α׷� �����
	glutDisplayFunc(DrawScene); //--- ��� �ݹ� �Լ�
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMainLoop();
}

//--- ��� �ݹ� �Լ�
void DrawScene() //--- glutDisplayFunc()�Լ��� ����� �׸��� �ݹ� �Լ�
{
	GLfloat rColor, gColor, bColor;
	rColor = bColor = 0.0;
	gColor = 1.0; //--- ������ �ʷϻ����� ����
	glClearColor(rColor, gColor, bColor, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaderProgramID);

	for (int i = 0; i < SPACE_END; ++i) {
		for (Line* line : vLines[i]) {
			glBindVertexArray(line->vao);
			glDrawArrays(GL_LINES, 0, 2); // �� �׸���
		}
	}

	// ��� �ﰢ���� �׸���
	for (int i = 0; i < SPACE_END; ++i) {
		for (Triangle* triangle : vTriangles[i]) {
			glBindVertexArray(triangle->vao);
			glDrawArrays(GL_TRIANGLES, 0, 3); // �ﰢ�� �׸���
		}
	}

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


GLuint Make_shaderProgram()
{
	GLuint shaderID = glCreateProgram(); //--- ���̴� ���α׷� �����
	GLint result;
	GLchar errorLog[512];

	glAttachShader(shaderID, vertexShader); //--- ���̴� ���α׷��� ���ؽ� ���̴� ���̱�
	glAttachShader(shaderID, fragmentShader); //--- ���̴� ���α׷��� �����׸�Ʈ ���̴� ���̱�
	glLinkProgram(shaderID); //--- ���̴� ���α׷� ��ũ�ϱ�


	glDeleteShader(vertexShader); //--- ���̴� ���α׷��� ��ũ�Ͽ� ���̴� ��ü ��ü�� ���� ����
	glDeleteShader(fragmentShader);

	glGetProgramiv(shaderID, GL_LINK_STATUS, &result); // ---���̴��� �� ����Ǿ����� üũ�ϱ�
	if (!result) {
		glGetProgramInfoLog(shaderID, 512, NULL, errorLog);
		std::cerr << "ERROR: shader program ���� ����\n" << errorLog << std::endl;
		return false;
	}
	return shaderID;
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

GLvoid Keyboard(unsigned char key, int x, int y){

	switch (key) {
	case 'l':
	
		break;
	case 't':


		break;
	case 'r':


		break;
	case 'p':


		break;
	case 'a':
		InitSet();
		break;
	default:
		break;
	
	}
	glutPostRedisplay();
}


void Make_Line(Point Point1, Point Point2,float* color,SPACE space) {

	Line* line = new Line;
	line->Point[POINT_LEFT].fpos[POS_X] = Point1.fpos[POS_X];
	line->Point[POINT_LEFT].fpos[POS_Y] = Point1.fpos[POS_Y];
	line->Point[POINT_LEFT].fpos[POS_Z] = 0.f;
	line->Point[POINT_RIGHT].fpos[POS_X] = Point2.fpos[POS_X];
	line->Point[POINT_RIGHT].fpos[POS_Y] = Point2.fpos[POS_Y];
	line->Point[POINT_RIGHT].fpos[POS_Z] = 0.f;

	glGenVertexArrays(1, &line->vao);
	glGenBuffers(VER_END, line->vbo);
	glBindVertexArray(line->vao);

	GLfloat* vertices = new GLfloat[2 * 3];
	GLfloat* colors = new GLfloat[2 * 3];

	vertices[0] = normalizeX(Point1.fpos[POS_X]);
	vertices[1] = normalizeY(Point1.fpos[POS_Y]);
	vertices[2] = 0.f;

	vertices[3] = normalizeX(Point2.fpos[POS_X]);
	vertices[4] = normalizeY(Point2.fpos[POS_Y]);
	vertices[5] = 0.f;

	colors[0] = color[COLOR_R];
	colors[1] = color[COLOR_G];
	colors[2] = color[COLOR_B];

	colors[3] = color[COLOR_R];
	colors[4] = color[COLOR_G];
	colors[5] = color[COLOR_B];

	glBindBuffer(GL_ARRAY_BUFFER, line->vbo[VER_POS]);
	glBufferData(GL_ARRAY_BUFFER, 2 * 3 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(VER_POS, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(VER_POS);

	glBindBuffer(GL_ARRAY_BUFFER, line->vbo[VER_COLOR]);
	glBufferData(GL_ARRAY_BUFFER, 2 * 3 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(VER_COLOR, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(VER_COLOR);

	delete[] vertices;
	delete[] colors;

	vLines[space].push_back(line);

}

void Make_Triangle(Point point, float* color,SPACE space,bool Delay) {

	Point point1;
	point1.fpos[POS_X] = point.fpos[POS_X] - 125.f;
	point1.fpos[POS_Y] = point.fpos[POS_Y] + 100.f;

	Point point2;
	point2.fpos[POS_X] = point.fpos[POS_X] + 125.f;
	point2.fpos[POS_Y] = point.fpos[POS_Y] + 100.f;

	Point point3;
	point3.fpos[POS_X] = point.fpos[POS_X] + 175.f;
	point3.fpos[POS_Y] = point.fpos[POS_Y];

	Make_Line(point1, point2, color, space);
	Make_Line(point2, point3, color, space);
	Make_Line(point3, point1, color, space);


	Triangle* triangle = new Triangle;

	triangle->Point[0].fpos[POS_X] = point1.fpos[POS_X];
	triangle->Point[0].fpos[POS_Y] = point1.fpos[POS_Y];
	triangle->Point[0].fpos[POS_Z] = 0.f;

	triangle->Point[1].fpos[POS_X] = point2.fpos[POS_X];
	triangle->Point[1].fpos[POS_Y] = point2.fpos[POS_Y];
	triangle->Point[1].fpos[POS_Z] = 0.f;

	triangle->Point[2].fpos[POS_X] = point3.fpos[POS_X];
	triangle->Point[2].fpos[POS_Y] = point3.fpos[POS_Y];
	triangle->Point[2].fpos[POS_Z] = 0.f;


	glGenVertexArrays(1, &triangle->vao);
	glGenBuffers(VER_END, triangle->vbo);
	glBindVertexArray(triangle->vao);

	GLfloat* vertices = new GLfloat[3 * 3];
	GLfloat* colors = new GLfloat[3 * 3];

	vertices[0] = normalizeX(point1.fpos[POS_X]);
	vertices[1] = normalizeY(point1.fpos[POS_Y]);
	vertices[2] = 0.f;

	vertices[3] = normalizeX(point2.fpos[POS_X]);
	vertices[4] = normalizeY(point2.fpos[POS_Y]);
	vertices[5] = 0.f;

	vertices[6] = normalizeX(point3.fpos[POS_X]);
	vertices[7] = normalizeY(point3.fpos[POS_Y]);
	vertices[8] = 0.f;

	colors[0] = color[COLOR_R];
	colors[1] = color[COLOR_G];
	colors[2] = color[COLOR_B];

	colors[3] = color[COLOR_R];
	colors[4] = color[COLOR_G];
	colors[5] = color[COLOR_B];

	colors[6] = color[COLOR_R];
	colors[7] = color[COLOR_G];
	colors[8] = color[COLOR_B];

	glBindBuffer(GL_ARRAY_BUFFER, triangle->vbo[VER_POS]);
	glBufferData(GL_ARRAY_BUFFER, 3 * 3 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(VER_POS, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(VER_POS);

	glBindBuffer(GL_ARRAY_BUFFER, triangle->vbo[VER_COLOR]);
	glBufferData(GL_ARRAY_BUFFER, 3 * 3 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(VER_COLOR, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(VER_COLOR);

	delete[] vertices;
	delete[] colors;

	vTriangles[space].push_back(triangle);
}

void Make_Square(Point point, float* color, SPACE space, bool Delay)
{

	Make_Triangle(point, color, space, false);

	Point point1;
	point1.fpos[POS_X] = point.fpos[POS_X] - 125.f;
	point1.fpos[POS_Y] = point.fpos[POS_Y] + 100.f;

	Point point3;
	point3.fpos[POS_X] = point.fpos[POS_X] + 175.f;
	point3.fpos[POS_Y] = point.fpos[POS_Y];

	Point point4;
	point4.fpos[POS_X] = point.fpos[POS_X] - 175.f;
	point4.fpos[POS_Y] = point.fpos[POS_Y];


	Make_Line(point3, point4, color, space);
	Make_Line(point4, point1, color, space);
	Make_Line(point1, point3, color, space);


	Triangle* triangle = new Triangle;

	triangle->Point[0].fpos[POS_X] = point1.fpos[POS_X];
	triangle->Point[0].fpos[POS_Y] = point1.fpos[POS_Y];
	triangle->Point[0].fpos[POS_Z] = 0.f;

	triangle->Point[1].fpos[POS_X] = point4.fpos[POS_X];
	triangle->Point[1].fpos[POS_Y] = point4.fpos[POS_Y];
	triangle->Point[1].fpos[POS_Z] = 0.f;

	triangle->Point[2].fpos[POS_X] = point3.fpos[POS_X];
	triangle->Point[2].fpos[POS_Y] = point3.fpos[POS_Y];
	triangle->Point[2].fpos[POS_Z] = 0.f;


	glGenVertexArrays(1, &triangle->vao);
	glGenBuffers(VER_END, triangle->vbo);
	glBindVertexArray(triangle->vao);

	GLfloat* vertices = new GLfloat[3 * 3];
	GLfloat* colors = new GLfloat[3 * 3];

	vertices[0] = normalizeX(point1.fpos[POS_X]);
	vertices[1] = normalizeY(point1.fpos[POS_Y]);
	vertices[2] = 0.f;

	vertices[3] = normalizeX(point4.fpos[POS_X]);
	vertices[4] = normalizeY(point4.fpos[POS_Y]);
	vertices[5] = 0.f;

	vertices[6] = normalizeX(point3.fpos[POS_X]);
	vertices[7] = normalizeY(point3.fpos[POS_Y]);
	vertices[8] = 0.f;

	colors[0] = color[COLOR_R];
	colors[1] = color[COLOR_G];
	colors[2] = color[COLOR_B];

	colors[3] = color[COLOR_R];
	colors[4] = color[COLOR_G];
	colors[5] = color[COLOR_B];

	colors[6] = color[COLOR_R];
	colors[7] = color[COLOR_G];
	colors[8] = color[COLOR_B];

	glBindBuffer(GL_ARRAY_BUFFER, triangle->vbo[VER_POS]);
	glBufferData(GL_ARRAY_BUFFER, 3 * 3 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(VER_POS, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(VER_POS);

	glBindBuffer(GL_ARRAY_BUFFER, triangle->vbo[VER_COLOR]);
	glBufferData(GL_ARRAY_BUFFER, 3 * 3 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(VER_COLOR, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(VER_COLOR);

	delete[] vertices;
	delete[] colors;

	vTriangles[space].push_back(triangle);

}

void Make_Pentagon(Point point, float* color, SPACE space)
{
	Make_Square(point, color, space, false);

	Point point5;
	point5.fpos[POS_X] = point.fpos[POS_X];
	point5.fpos[POS_Y] = point.fpos[POS_Y] - 100.f;

	Point point3;
	point3.fpos[POS_X] = point.fpos[POS_X] + 175.f;
	point3.fpos[POS_Y] = point.fpos[POS_Y];

	Point point4;
	point4.fpos[POS_X] = point.fpos[POS_X] - 175.f;
	point4.fpos[POS_Y] = point.fpos[POS_Y];


	Make_Line(point3, point4, color, space);
	Make_Line(point4, point5, color, space);
	Make_Line(point5, point3, color, space);
	

	Triangle* triangle = new Triangle;

	triangle->Point[0].fpos[POS_X] = point5.fpos[POS_X];
	triangle->Point[0].fpos[POS_Y] = point5.fpos[POS_Y];
	triangle->Point[0].fpos[POS_Z] = 0.f;

	triangle->Point[1].fpos[POS_X] = point4.fpos[POS_X];
	triangle->Point[1].fpos[POS_Y] = point4.fpos[POS_Y];
	triangle->Point[1].fpos[POS_Z] = 0.f;

	triangle->Point[2].fpos[POS_X] = point3.fpos[POS_X];
	triangle->Point[2].fpos[POS_Y] = point3.fpos[POS_Y];
	triangle->Point[2].fpos[POS_Z] = 0.f;


	glGenVertexArrays(1, &triangle->vao);
	glGenBuffers(VER_END, triangle->vbo);
	glBindVertexArray(triangle->vao);

	GLfloat* vertices = new GLfloat[3 * 3];
	GLfloat* colors = new GLfloat[3 * 3];

	vertices[0] = normalizeX(point5.fpos[POS_X]);
	vertices[1] = normalizeY(point5.fpos[POS_Y]);
	vertices[2] = 0.f;

	vertices[3] = normalizeX(point4.fpos[POS_X]);
	vertices[4] = normalizeY(point4.fpos[POS_Y]);
	vertices[5] = 0.f;

	vertices[6] = normalizeX(point3.fpos[POS_X]);
	vertices[7] = normalizeY(point3.fpos[POS_Y]);
	vertices[8] = 0.f;

	colors[0] = color[COLOR_R];
	colors[1] = color[COLOR_G];
	colors[2] = color[COLOR_B];

	colors[3] = color[COLOR_R];
	colors[4] = color[COLOR_G];
	colors[5] = color[COLOR_B];

	colors[6] = color[COLOR_R];
	colors[7] = color[COLOR_G];
	colors[8] = color[COLOR_B];

	glBindBuffer(GL_ARRAY_BUFFER, triangle->vbo[VER_POS]);
	glBufferData(GL_ARRAY_BUFFER, 3 * 3 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(VER_POS, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(VER_POS);

	glBindBuffer(GL_ARRAY_BUFFER, triangle->vbo[VER_COLOR]);
	glBufferData(GL_ARRAY_BUFFER, 3 * 3 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(VER_COLOR, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(VER_COLOR);

	delete[] vertices;
	delete[] colors;

	vTriangles[space].push_back(triangle);
}

void InitSet()
{

	//1��и�
	Point point1;
	point1.fpos[POS_X] = SPACE1CENTERX - 125.f;
	point1.fpos[POS_Y] = SPACE1CENTERX + 100.f;

	Point point2;
	point2.fpos[POS_X] = SPACE1CENTERX + 125.f;
	point2.fpos[POS_Y] = SPACE1CENTERX + 100.f;

	float color[COLOR_END];
	color[COLOR_R] = 1.f;
	color[COLOR_G] = 0.f;
	color[COLOR_B] = 0.f;
	Make_Line(point1, point2, color, SPACE_1);

	//2��и�
	Point point3;
	point3.fpos[POS_X] = SPACE2CENTERX;
	point3.fpos[POS_Y] = SPACE2CENTERX;
	Make_Triangle(point3, color, SPACE_2, true);


	//3��и�

	Point point4;
	point4.fpos[POS_X] = SPACE2CENTERX;
	point4.fpos[POS_Y] = SPACE2CENTERX;
	Make_Square(point3, color, SPACE_3, true);

	//4��и�

	Point point5;
	point5.fpos[POS_X] = SPACE2CENTERX;
	point5.fpos[POS_Y] = SPACE2CENTERX;
	Make_Pentagon(point3, color, SPACE_4);


}

