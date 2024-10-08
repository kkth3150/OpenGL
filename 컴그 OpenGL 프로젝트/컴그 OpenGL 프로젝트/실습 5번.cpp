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

	float rect_size = 10;
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


	my_rect() {}
	my_rect(float fx, float fy, float fsize) : x(fx), y(fy), rect_size(fsize) {}
};

my_rect eraser(300, 400, 30);






int main(int argc, char** argv) { //--- ������ ����ϰ� �ݹ��Լ� ���� { //--- ������ �����ϱ�
	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ����
	glutInitWindowPosition(100, 100); // �������� ��ġ ����
	glutInitWindowSize(800, 600); // �������� ũ�� ����
	glutCreateWindow("�ǽ� 5"); // ������ ����(������ �̸�)

	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew �ʱ�ȭ
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	for (int i = 0; i < rand() % 20 + 20; ++i) {
		my_rect new_rect;
		rects.push_back(new_rect);
	}
	eraser.set_color();

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
	if (left_button) {
		glColor3f(eraser.red, eraser.green, eraser.blue);
		glRectf(normalizeX(eraser.x - eraser.rect_size), normalizeY(eraser.y - eraser.rect_size),
			normalizeX(eraser.x + eraser.rect_size), normalizeY(eraser.y + eraser.rect_size));
	}

	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}

GLvoid Reshape(int w, int h) { //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
	glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y)
{

	switch (key) {
	case 'r':
		rects.clear();
		for (int i = 0; i < rand() % 20 + 20; ++i) {
			my_rect new_rect;
			rects.push_back(new_rect);
		}
		break;

	case 'q':
		exit(0);
		break;
	}
	glutPostRedisplay();
}




void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			left_button = true;
			eraser.x = x;
			eraser.y = y;
		}
		else if (state == GLUT_UP) {
			left_button = false;
			eraser.rect_size = 30;
			eraser.set_color();

		}
	}


}

void Motion(int mx, int my) {
	if (left_button == true) {
		eraser.x = mx;
		eraser.y = my;
		drawScene();
		for (int i = 0; i < rects.size(); ++i) {
			if ((rects[i].x - eraser.x) < (eraser.rect_size + 10) && (rects[i].x - eraser.x) > -(eraser.rect_size + 10)) {
				if (rects[i].y - eraser.y < eraser.rect_size + 10 && rects[i].y - eraser.y > -eraser.rect_size + 10) {
					eraser.blue = rects[i].blue;
					eraser.green = rects[i].green;
					eraser.red = rects[i].red;
					eraser.rect_size += 5;

					rects.erase(rects.begin() + i);
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
