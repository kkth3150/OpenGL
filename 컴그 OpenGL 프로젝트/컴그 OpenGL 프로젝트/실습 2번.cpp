#include "library.h"
#include <iostream>

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid KeyBoard(unsigned char key, int x, int y);

void TimerFunction(int value);
float RGB[3] = { 0.f,0.f,0.f };
bool timerON = false;

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ���� 
{ //--- ������ �����ϱ�
	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);// ���÷��� ��� ����
	glutInitWindowPosition(100, 100); // �������� ��ġ ����
	glutInitWindowSize(800, 600);// �������� ũ�� ����
	glutCreateWindow("Example1"); // ������ ����(������ �̸�)
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

	glutKeyboardFunc(KeyBoard);

	glutMainLoop(); // �̺�Ʈ ó�� ����
}

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ� 
{

	glClearColor(RGB[0], RGB[1], RGB[2], 1.0f); // �������� ��blue�� �� ����
	glClear(GL_COLOR_BUFFER_BIT); // ������ ������ ��ü�� ĥ�ϱ�// �׸��� �κ� ����: �׸��� ���� �κ��� ���⿡ ���Եȴ�.
	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}

GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ� 
{
	glViewport(0, 0, w, h);
}

GLvoid KeyBoard(unsigned char key, int x, int y) {



	switch (key) {
	case 'c':
		RGB[0] = 0.f;
		RGB[1] = 1.f;
		RGB[2] = 1.f;
		break;
	case 'm':
		RGB[0] = 1.f;
		RGB[1] = 0.f;
		RGB[2] = 1.f;
		break;
	case 'y':
		RGB[0] = 1.f;
		RGB[1] = 1.f;
		RGB[2] = 0.f;
		break;
	case 'a':
		RGB[0] = (float)(rand() % 100) / 100;
		RGB[1] = (float)(rand() % 100) / 100;
		RGB[2] = (float)(rand() % 100) / 100;
		break;
	case 'w':
		RGB[0] = 1.f;
		RGB[1] = 1.f;
		RGB[2] = 1.f;
		break;
	case 'k':
		RGB[0] = 0.f;
		RGB[1] = 0.f;
		RGB[2] = 0.f;
		break;
	case 't':
		timerON = true;
		glutTimerFunc(1000, TimerFunction, 1);
		break;
	case 's':
		timerON = false;
		break;
	case 'q':
		return;
	}
	glutPostRedisplay();

}

void TimerFunction(int value)
{
	if (timerON) {
		RGB[0] = (float)(rand() % 100) / 100;
		RGB[1] = (float)(rand() % 100) / 100;
		RGB[2] = (float)(rand() % 100) / 100;
		glutPostRedisplay(); // ȭ�� �� ���
		glutTimerFunc(1000, TimerFunction, 1); // Ÿ�̸��Լ� �� ����
	}
}