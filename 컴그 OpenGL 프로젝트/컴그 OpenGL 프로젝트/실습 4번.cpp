#include "library.h"
#include <iostream>
#include <random>
#include <vector>
using namespace std;

default_random_engine dre;
uniform_int_distribution<> uidx(0, 800);
uniform_int_distribution<> uidy(0, 600);
uniform_real_distribution<> urd(0.0f, 1.0f);
uniform_int_distribution<> uid_size(1, 20);

class my_rect;

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char, int, int);

void Motion(int, int);
void TimerFunction_1(int);
void TimerFunction_3(int);
void TimerFunction_4(int);
void Mouse(int, int, int, int);

GLfloat normalizeX(int);
GLfloat normalizeY(int);


bool timer = false;
bool left_button = false;

vector<my_rect> rects = {};
size_t rect_index = 0;

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

	float rect_size = 30;
	bool size_PorM = rand() % 2;

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

	void move_1() {

		if (x + rect_size > 800) {
			x -= rect_size;
			dirx = -dirx;
		}
		if (x - rect_size < 0) {
			x += rect_size;
			dirx = -dirx;
		}
		if (y + rect_size > 600) {
			y -= rect_size;
			diry = -diry;
		}
		if (y - rect_size < 0) {
			y += rect_size;
			diry = -diry;
		}
		x += dirx;
		y += diry;
	}


	my_rect() {}
	my_rect(float fx, float fy) : x(fx), y(fy) {}
};





int main(int argc, char** argv) { //--- ������ ����ϰ� �ݹ��Լ� ���� { //--- ������ �����ϱ�
	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ����
	glutInitWindowPosition(100, 100); // �������� ��ġ ����
	glutInitWindowSize(800, 600); // �������� ũ�� ����
	glutCreateWindow("�ǽ� 4"); // ������ ����(������ �̸�)

	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew �ʱ�ȭ
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";


	glutDisplayFunc(drawScene); // ��� �Լ��� ����
	glutReshapeFunc(Reshape); // �ٽ� �׸��� �Լ� ����
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutMainLoop(); // �̺�Ʈ ó�� ����
}

GLvoid drawScene() {						//--- �ݹ� �Լ�: �׸��� �ݹ� �Լ�
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);		// ������ ������ ��ü�� ĥ�ϱ�
	// �׸��� �κ� ����: �׸��� ���� �κ��� ���⿡ ���Եȴ�

	for (my_rect r : rects) {
		glColor3f(r.red, r.green, r.blue);
		glRectf(normalizeX(r.x - r.rect_size), normalizeY(r.y - r.rect_size), normalizeX(r.x + r.rect_size), normalizeY(r.y + r.rect_size));
	}

	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}

GLvoid Reshape(int w, int h) { //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
	glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y)
{

	switch (key) {
	case '1': //�밢��
		for (my_rect& mr : rects) {
			mr.dirx = 10;
			mr.diry = 10;
		}
		if (move1_state) {
			move1_state = false;
		}
		else {
			move1_state = true;
			glutTimerFunc(10, TimerFunction_1, 1);
		}

		break;
	case '2': // �������
		for (my_rect& mr : rects) {
			mr.dirx = 30;
			mr.diry = 3;
		}
		if (move1_state) {
			move1_state = false;
		}
		else {
			move1_state = true;
			glutTimerFunc(10, TimerFunction_1, 1);
		}

		break;
	case '3': // ũ��
		if (size_state) {
			size_state = false;
		}
		else {
			size_state = true;
			glutTimerFunc(1000, TimerFunction_3, 1);
		}
		break;
	case '4': // ����
		if (color_state) {
			color_state = false;
		}
		else {
			color_state = true;
			glutTimerFunc(1000, TimerFunction_4, 1);
		}
		break;
	case 's': // ����
		for (my_rect& mr : rects) {
			mr.dirx = 0;
			mr.diry = 0;
			color_state = false;
			size_state = false;
			timer = false;
		}
		break;
	case 'm': // ó����ġ��
		for (my_rect& mr : rects) {
			mr.x = mr.first_x;
			mr.y = mr.first_y;
		}
		break;
	case 'r': // �� ����� �ٽ�
		rects.clear();
		break;
	case 'q':
		exit(0);
		break;
	}
	glutPostRedisplay();
}

void TimerFunction_1(int value) {
	for (my_rect& mr : rects) {
		mr.move_1();
	}
	glutPostRedisplay();
	if (move1_state) {
		glutTimerFunc(20, TimerFunction_1, 1);
	}
}


void TimerFunction_3(int value) {
	for (my_rect& mr : rects) {
		mr.size_change();

	}
	glutPostRedisplay();
	if (size_state) {
		glutTimerFunc(100, TimerFunction_3, 1);
	}
}

void TimerFunction_4(int value) {
	for (my_rect& mr : rects) {
		mr.set_color();
	}
	glutPostRedisplay();
	if (color_state) {
		glutTimerFunc(100, TimerFunction_4, 1);
	}
}


void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		left_button = true;
		my_rect rect_a(x, y);
		if (rects.size() < 5)
			rects.push_back(rect_a);
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
