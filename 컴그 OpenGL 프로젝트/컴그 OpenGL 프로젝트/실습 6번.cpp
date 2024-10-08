#include "library.h"
#include <iostream>
#include <random>
#include <vector>
using namespace std;

default_random_engine dre;
uniform_int_distribution<> uidx(50, 750);
uniform_int_distribution<> uidy(50, 550);
uniform_real_distribution<> urd(0.0f, 1.0f);
uniform_int_distribution<> uid_size(1, 20);

class my_rect;

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char, int, int);

void Motion(int, int);
void TimerFunction_1(int);
void Mouse(int, int, int, int);

GLfloat normalizeX(int);
GLfloat normalizeY(int);


bool timer = false;
bool left_button = false;

vector<my_rect> rects = {};
vector<my_rect> mini_rects = {};
size_t rect_index = 0;

int move_kind = -1;

bool color_state = false;
bool size_state = false;
bool move1_state = false;

class my_rect
{// x 0~800 y 0~600
public:
	size_t index = rect_index++;

	float x = uidx(dre);
	float y = uidy(dre);
	float first_x = x;
	float first_y = y;

	float dirx = 10;
	float diry = 10;

	float red = urd(dre);
	float green = urd(dre);
	float blue = urd(dre);

	float rect_size = rand() % 20 + 10;
	bool size_PorM = rand() % 2;

	bool isinrect(int mx, int my) {
		if (mx > x - rect_size && mx < x + rect_size) {
			if (my > y - rect_size && my < y + rect_size) {
				return true;
			}
		}
		return false;
	}
	void set_color() {
		red = 0.0f;
		green = 0.0f;
		blue = 0.0f;
	}

	void size_change() {
		if (size_PorM) {
			rect_size += uid_size(dre);
			if (rect_size > 100) {
				size_PorM = false;
			}
		}
		else {
			rect_size -= uid_size(dre);
			if (rect_size < 20) {
				size_PorM = true;
			}
		}
	}

	void move(int move) {
		switch (move)
		{
		case 0:
			if (index % 4 == 0) {
				x += dirx*2;
			}
			else if (index % 4 == 1) {
				x -= dirx * 2;
			}
			else if (index % 4 == 2) {
				y += dirx*2;
			}
			else if (index % 4 == 3) {
				y -= dirx*2;
			}
			break;
		case 1: //대각선
			if (index % 4 == 0) {
				x += dirx;
				y += dirx;
			}
			else if (index % 4 == 1) {
				x -= dirx;
				y += dirx;
			}
			else if (index % 4 == 2) {
				x += dirx;
				y -= dirx;
			}
			else if (index % 4 == 3) {
				x -= dirx;
				y -= dirx;
			}
			break;
		case 2: //한쪽방향
			x += dirx*2;
			break;
		case 3: //상하좌우 대각선
			if (index % 8 == 0) {
			
				x += dirx;
			}
			else if (index % 8 == 1) {
				x -= dirx;
			}
			else if (index % 8 == 2) {
				y += dirx;
			}
			else if (index % 8 == 3) {
				y -= dirx;
			}
			else if (index % 8 == 4) {
			
				x += dirx;
				y += dirx;
			}
			else if (index % 8 == 5) {
				x -= dirx;
				y += dirx;
			}
			else if (index % 8 == 6) {
				x += dirx;
				y -= dirx;
			}
			else if (index % 8 == 7) {
				x -= dirx;
				y -= dirx;
			}
			break;
		}
	}

	my_rect() {}
	my_rect(float fx, float fy, float fsize) : x(fx), y(fy), rect_size(fsize) {}
	my_rect(float fsize) : rect_size(fsize) {};
	my_rect(float fx, float fy, float fsize, float fblue, float fgreen, float fred)
		: x(fx), y(fy), rect_size(fsize), blue(fblue), green(fgreen), red(fred) {}
};







int main(int argc, char** argv) { //--- 윈도우 출력하고 콜백함수 설정 { //--- 윈도우 생성하기
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
	glutInitWindowPosition(100, 100); // 윈도우의 위치 지정
	glutInitWindowSize(800, 600); // 윈도우의 크기 지정
	glutCreateWindow("Example1"); // 윈도우 생성(윈도우 이름)

	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew 초기화
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	for (int i = 0; i < 5 + rand() % 5; ++i) {
		my_rect new_rect;
		rects.push_back(new_rect);
	}
	rect_index = 0;
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

	for (const my_rect r : rects) {
		glColor3f(r.red, r.green, r.blue);
		glRectf(normalizeX(r.x - r.rect_size), normalizeY(r.y - r.rect_size),
			normalizeX(r.x + r.rect_size), normalizeY(r.y + r.rect_size));
	}
	for (my_rect r : mini_rects) {
		glColor3f(r.red, r.green, r.blue);
		glRectf(normalizeX(r.x - r.rect_size), normalizeY(r.y - r.rect_size),
			normalizeX(r.x + r.rect_size), normalizeY(r.y + r.rect_size));
	}

	glutSwapBuffers(); // 화면에 출력하기
}

GLvoid Reshape(int w, int h) { //--- 콜백 함수: 다시 그리기 콜백 함수
	glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y)
{

	switch (key) {
	case 'r':
		for (int i = 0; i < 5 + rand() % 5; ++i) {
			my_rect new_rect;
			rects.push_back(new_rect);
		}
		rect_index = 0;
		break;
	case 'q':
		exit(0);
		break;
	}
	glutPostRedisplay();
}

void TimerFunction_1(int value) {
	for (int j = 0; j < mini_rects.size(); ++j) {
		mini_rects[j].move(move_kind);
		if (mini_rects[j].rect_size > 1) {
			mini_rects[j].rect_size -= 0.5;
		}
		else {
			mini_rects.erase(mini_rects.begin() + j);
		}
	}

	glutPostRedisplay();
	if (mini_rects.size()) {
		glutTimerFunc(100, TimerFunction_1, 1);
	}
}

void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			left_button = true;
			for (int i = 0; i < rects.size(); ++i) {
				if (rects[i].isinrect(x, y)) {
					move_kind = (move_kind + 1) % 4;
					if (move_kind < 3) {
						my_rect new_mini_rect1(rects[i].x - rects[i].rect_size / 2,
							rects[i].y - rects[i].rect_size / 2, rects[i].rect_size / 2,
							rects[i].blue, rects[i].green, rects[i].red);
						my_rect new_mini_rect2(rects[i].x + rects[i].rect_size / 2,
							rects[i].y - rects[i].rect_size / 2, rects[i].rect_size / 2,
							rects[i].blue, rects[i].green, rects[i].red);
						my_rect new_mini_rect3(rects[i].x - rects[i].rect_size / 2,
							rects[i].y + rects[i].rect_size / 2, rects[i].rect_size / 2,
							rects[i].blue, rects[i].green, rects[i].red);
						my_rect new_mini_rect4(rects[i].x + rects[i].rect_size / 2,
							rects[i].y + rects[i].rect_size / 2, rects[i].rect_size / 2,
							rects[i].blue, rects[i].green, rects[i].red);
						mini_rects.push_back(new_mini_rect1);
						mini_rects.push_back(new_mini_rect2);
						mini_rects.push_back(new_mini_rect3);
						mini_rects.push_back(new_mini_rect4);
						rects.erase(rects.begin() + i);
						glutTimerFunc(20, TimerFunction_1, 1);

					}
					else {
						my_rect new_mini_rect1(rects[i].x - rects[i].rect_size / 2,
							rects[i].y - rects[i].rect_size / 2, rects[i].rect_size / 2,
							rects[i].blue, rects[i].green, rects[i].red);
						my_rect new_mini_rect2(rects[i].x + rects[i].rect_size / 2,
							rects[i].y - rects[i].rect_size / 2, rects[i].rect_size / 2,
							rects[i].blue, rects[i].green, rects[i].red);
						my_rect new_mini_rect3(rects[i].x - rects[i].rect_size / 2,
							rects[i].y + rects[i].rect_size / 2, rects[i].rect_size / 2,
							rects[i].blue, rects[i].green, rects[i].red);
						my_rect new_mini_rect4(rects[i].x + rects[i].rect_size / 2,
							rects[i].y + rects[i].rect_size / 2, rects[i].rect_size / 2,
							rects[i].blue, rects[i].green, rects[i].red);
						my_rect new_mini_rect5(rects[i].x - rects[i].rect_size / 2,
							rects[i].y - rects[i].rect_size / 2, rects[i].rect_size / 2,
							rects[i].blue, rects[i].green, rects[i].red);
						my_rect new_mini_rect6(rects[i].x + rects[i].rect_size / 2,
							rects[i].y - rects[i].rect_size / 2, rects[i].rect_size / 2,
							rects[i].blue, rects[i].green, rects[i].red);
						my_rect new_mini_rect7(rects[i].x - rects[i].rect_size / 2,
							rects[i].y + rects[i].rect_size / 2, rects[i].rect_size / 2,
							rects[i].blue, rects[i].green, rects[i].red);
						my_rect new_mini_rect8(rects[i].x + rects[i].rect_size / 2,
							rects[i].y + rects[i].rect_size / 2, rects[i].rect_size / 2,
							rects[i].blue, rects[i].green, rects[i].red);
						mini_rects.push_back(new_mini_rect1);
						mini_rects.push_back(new_mini_rect2);
						mini_rects.push_back(new_mini_rect3);
						mini_rects.push_back(new_mini_rect4);
						mini_rects.push_back(new_mini_rect5);
						mini_rects.push_back(new_mini_rect6);
						mini_rects.push_back(new_mini_rect7);
						mini_rects.push_back(new_mini_rect8);
						rects.erase(rects.begin() + i);
						glutTimerFunc(20, TimerFunction_1, 1);
					}

				}
				drawScene();
			}

		}

	}
}

void Motion(int mx, int my) {
	if (left_button == true) {


	}

}

GLfloat normalizeX(int x) {
	return (x / 400.0f) - 1.0f;
}

GLfloat normalizeY(int y) {
	return -((y / 300.0f) - 1.0f);
}
