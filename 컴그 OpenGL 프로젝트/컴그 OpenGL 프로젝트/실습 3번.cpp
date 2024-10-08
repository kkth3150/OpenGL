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
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
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
