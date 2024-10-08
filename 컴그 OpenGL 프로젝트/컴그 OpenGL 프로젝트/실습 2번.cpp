#include "library.h"
#include <iostream>
#include <random>
using namespace std;

default_random_engine dre;
uniform_real_distribution<> urd(0.0f, 1.0f);

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char, int, int);

void TimerFunction(int);
void Mouse(int, int, int, int);
void set_color1();
void set_color2();
void set_color3();
void set_color4();
void set_color5();
void set_color6();
void set_color7();
void set_color8();

float red1 = urd(dre);
float green1 = urd(dre);
float blue1 = urd(dre);

float red2 = urd(dre);
float green2 = urd(dre);
float blue2 = urd(dre);

float red3 = urd(dre);
float green3 = urd(dre);
float blue3 = urd(dre);

float red4 = urd(dre);
float green4 = urd(dre);
float blue4 = urd(dre);


float red5 = urd(dre);
float green5 = urd(dre);
float blue5 = urd(dre);

float red6 = urd(dre);
float green6 = urd(dre);
float blue6 = urd(dre);

float red7 = urd(dre);
float green7 = urd(dre);
float blue7 = urd(dre);

float red8 = urd(dre);
float green8 = urd(dre);
float blue8 = urd(dre);

float size1 = 0.0f;
float size2 = 0.0f;
float size3 = 0.0f;
float size4 = 0.0f;


bool timer = false;



int main(int argc, char** argv) { //--- 윈도우 출력하고 콜백함수 설정 { //--- 윈도우 생성하기
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
	glutInitWindowPosition(100, 100); // 윈도우의 위치 지정
	glutInitWindowSize(800, 600); // 윈도우의 크기 지정
	glutCreateWindow("실습 2"); // 윈도우 생성(윈도우 이름)

	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew 초기화
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";


	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMainLoop(); // 이벤트 처리 시작
}

GLvoid drawScene() {						//--- 콜백 함수: 그리기 콜백 함수
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);		// 설정된 색으로 전체를 칠하기
	// 그리기 부분 구현: 그리기 관련 부분이 여기에 포함된다

	glColor3f(red1, green1, blue1);
	glRectf(-1.0f, 0.0f, 0.0f, 1.0f);
	glColor3f(red2, green2, blue2);
	glRectf(-1.0f, -1.0f, 0.0f, 0.0f);
	glColor3f(red3, green3, blue3);
	glRectf(0.0f, 0.0f, 1.0f, 1.0f);
	glColor3f(red4, green4, blue4);
	glRectf(0.0f, -1.0f, 1.0f, 0.0f);

	glColor3f(red5, green5, blue5);
	glRectf(-0.75f + size1, 0.25f + size1, -0.25f - size1, 0.75f - size1);
	glColor3f(red6, green6, blue6);
	glRectf(-0.75f + size2, -0.75f + size2, -0.25f - size2, -0.25f - size2);
	glColor3f(red7, green7, blue7);
	glRectf(0.25f + size3, 0.25f + size3, 0.75f - size3, 0.75f - size3);
	glColor3f(red8, green8, blue8);
	glRectf(0.25f + size4, -0.75f + size4, 0.75f - size4, -0.25f - size4);


	glutSwapBuffers(); // 화면에 출력하기
}

GLvoid Reshape(int w, int h) { //--- 콜백 함수: 다시 그리기 콜백 함수
	glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'q':
		exit(0);
		break;
	}
	glutPostRedisplay();
}

void TimerFunction(int value) {
	set_color1();

	glutPostRedisplay();
	if (timer) {
		glutTimerFunc(1000, TimerFunction, 1);
	}
}

void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (x < 400 && y < 300) {
			if (x < 300 && x > 100 && y < (600 / 8) * 3 && y >(600 / 8)) {
				set_color5();
			}
			else
				set_color1();
		}
		else if (x < 400 && y >300) {
			if (x < 300 && x > 100 && y < (600 / 8) * 7 && y >(600 / 8) * 5) {
				set_color6();
			}
			else
				set_color2();
		}
		else if (x > 400 && y < 300) {
			if (x < 700 && x > 500 && y < (600 / 8) * 3 && y >(600 / 8)) {
				set_color7();
			}
			else
				set_color3();
		}
		else if (x > 400 && y > 300) {
			if (x < 700 && x > 500 && y < (600 / 8) * 7 && y >(600 / 8) * 5) {
				set_color8();
			}
			else
				set_color4();
		}
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		
		if (x < 400 && y < 300) {
			if (x < 300 && x > 100 && y < (600 / 8) * 3 && y >(600 / 8)) {
				size1 += 0.01f;
			}
			else
				size1 -= 0.01f;
		}
		else if (x < 400 && y >300) {
			if (x < 300 && x > 100 && y < (600 / 8) * 7 && y >(600 / 8) * 5) {
				size2 += 0.01f;
			}
			else
				size2 -= 0.01f;
		}
		else if (x > 400 && y < 300) {
			if (x < 700 && x > 500 && y < (600 / 8) * 3 && y >(600 / 8)) {
				size3 += 0.01f;
			}
			else
				size3 -= 0.01;
		}
		else if (x > 400 && y > 300) {
			if (x < 700 && x > 500 && y < (600 / 8) * 7 && y >(600 / 8) * 5) {
				size4 += 0.01;
			}
			else
				size4 -= 0.01;
		}

	}
}

void set_color1() {
	red1 = urd(dre);
	green1 = urd(dre);
	blue1 = urd(dre);
}
void set_color2() {
	red2 = urd(dre);
	green2 = urd(dre);
	blue2 = urd(dre);
}
void set_color3() {
	red3 = urd(dre);
	green3 = urd(dre);
	blue3 = urd(dre);
}
void set_color4() {
	red4 = urd(dre);
	green4 = urd(dre);
	blue4 = urd(dre);
}

void set_color5() {
	red5 = urd(dre);
	green5 = urd(dre);
	blue5 = urd(dre);
}
void set_color6() {
	red6 = urd(dre);
	green6 = urd(dre);
	blue6 = urd(dre);
}
void set_color7() {
	red7 = urd(dre);
	green7 = urd(dre);
	blue7 = urd(dre);
}
void set_color8() {
	red8 = urd(dre);
	green8 = urd(dre);
	blue8 = urd(dre);
}
