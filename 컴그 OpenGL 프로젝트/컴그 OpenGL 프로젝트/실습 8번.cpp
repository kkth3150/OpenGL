#define _CRT_SECURE_NO_WARNINGS //--- ���α׷� �� �տ� ������ ��
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

enum Dir {
	DIR_UP, DIR_DOWN, DIR_RIGHT, DIR_LEFT
};

enum Face {
	FACE_LT, FACE_RT, FACE_RB, FACE_LB, FACE_END
};



struct Triangle {
	GLuint vao;
	GLuint vbo[VER_END];
	float color[3];
	float pos[3];
	float size;
	bool isLine;

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
	rColor = bColor = 1.0;
	gColor = 1.0; //--- ������ �ʷϻ����� ����
	glClearColor(rColor, gColor, bColor, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaderProgramID);

	for (const auto& shape : vTriangles) {
		glBindVertexArray(shape.vao); // ������ VAO ���ε�
		if(shape.isLine)
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 3); // �� ������ �׸���
		else {
			glDrawArrays(GL_LINE_LOOP, 0, 3);
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
	case 'a':
		TriangleMode = true;
		break;

	case 'b':
		TriangleMode = false;
		break;

	case 'q':
		exit(0);
		break;
	}
	glutPostRedisplay();
}

void Add_Shape(int x,int y) {
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



void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		Delete_Shape(which_face(x, y));
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

		Add_Shape(x,y);
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
