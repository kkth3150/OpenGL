#include "library.h"
#include <iostream>
#include <random>
#include <vector>
using namespace std;

default_random_engine dre;
uniform_int_distribution<> uidx(0, 800);
uniform_int_distribution<> uidy(0, 600);
uniform_real_distribution<> urd(0.0f, 1.0f);

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char, int, int);
void Motion(int, int);
void TimerFunction(int);
void Mouse(int, int, int, int);
GLfloat normalizeX(int);

GLfloat normalizeY(int);

size_t rect_index = 0;

class my_rect
{// x 0~800 y 0~600
public:
	size_t index = rect_index++;

	float x = uidx(dre);
	float y = uidy(dre);

	float red = urd(dre);
	float green = urd(dre);
	float blue = urd(dre);

	float rect_size = 30;

	bool isinrect(int mx, int my) {
		if (mx > x - rect_size && mx < x + rect_size) {
			if (my > y - rect_size && y < y + rect_size) {
				return true;
			}
		}
		return false;
	}
	void set_color() {
		red = urd(dre);
		green = urd(dre);
		blue = urd(dre);
	}
};


bool timer = false;
bool left_button = false;

vector<my_rect> rects = {};



int main(int argc, char** argv) { //--- 윈도우 출력하고 콜백함수 설정 { //--- 윈도우 생성하기
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
	glutInitWindowPosition(100, 100); // 윈도우의 위치 지정
	glutInitWindowSize(800, 600); // 윈도우의 크기 지정
	glutCreateWindow("실습 4"); // 윈도우 생성(윈도우 이름)

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
	glutMotionFunc(Motion);
	glutMainLoop(); // 이벤트 처리 시작
}

GLvoid drawScene() {						//--- 콜백 함수: 그리기 콜백 함수
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);		// 설정된 색으로 전체를 칠하기
	// 그리기 부분 구현: 그리기 관련 부분이 여기에 포함된다

	for (my_rect r : rects) {
		glColor3f(r.red, r.green, r.blue);
		glRectf(normalizeX(r.x - r.rect_size), normalizeY(r.y - r.rect_size), normalizeX(r.x + r.rect_size), normalizeY(r.y + r.rect_size));
	}

	glutSwapBuffers(); // 화면에 출력하기
}

GLvoid Reshape(int w, int h) { //--- 콜백 함수: 다시 그리기 콜백 함수
	glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	my_rect rect_a;
	switch (key) {
	case 'a':
		if (rects.size() < 11)
			rects.push_back(rect_a);
		break;
	case 'q':
		exit(0);
		break;
	}
	glutPostRedisplay();
}

void TimerFunction(int value) {

	glutPostRedisplay();
	if (timer) {
		glutTimerFunc(1000, TimerFunction, 1);
	}
}

void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		left_button = true;
	}

}

void Motion(int mx, int my) {
	if (left_button == true) {
		for (int j = 0; j < rects.size(); ++j) {
			if (rects[j].isinrect(mx, my)) {
				rects[j].x = mx;
				rects[j].y = my;
				drawScene();
			}
			for (int i = 0; i < rects.size(); ++i) {
				if (rects[j].x == rects[i].x && rects[j].y == rects[i].y && rects[j].index != rects[i].index) {
					if (rects[j].rect_size >= rects[i].rect_size) {
						rects[j].rect_size += 10;
						rects[j].set_color();
						rects.erase(rects.begin() + i);
					}
					else {
						rects[i].rect_size += 10;
						rects[i].set_color();
						rects.erase(rects.begin() + j);
					}

					i--;
					j--;
				}
			}
		}

	}

}

GLfloat normalizeX(int x) {
	return (x / 400.0f) - 1.0f;
}

GLfloat normalizeY(int y) {
	return -((y / 300.0f) - 1.0f);
}
